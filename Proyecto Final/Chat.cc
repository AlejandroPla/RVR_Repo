#include "Chat.h"
#include <algorithm>
#include <string>
#include <string.h>
void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);
    memset(_data, 0, MESSAGE_SIZE);
	char* dt = _data;
    memcpy(dt, &type, sizeof(uint8_t));
	dt += sizeof(uint8_t);
	for (unsigned int i = 0; i < 7 && i < nick.length(); i++, dt++) {
	 *dt = nick[i];
	}
}
int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);
    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);
	type = (uint8_t) *_data;
	
	char * _nick = _data + sizeof(uint8_t);
	
	std::string n(_nick, 8);
	nick = n;
    return 0;
}
void ChatServer::do_messages()
{
    while (true)
    {
		ChatMessage message;
		Socket* sdMessage;
		socket.recv(message, sdMessage);
		switch(message.type) {
			case ChatMessage::UP:
				if (message.nick == player1) {
					game->movePlayer(false, 0, -10);
				} else if (message.nick == player2) {
					game->movePlayer(true, 0, -10);
				}			
				break;
			case ChatMessage::DOWN:
				if (message.nick == player1) {
					game->movePlayer(false, 0, 10);
				} else if (message.nick == player2) {
					game->movePlayer(true, 0, 10);
				}			
			break;
			case ChatMessage::LEFT:
				if (message.nick == player1) {
					game->movePlayer(false, -10, 0);
				} else if (message.nick == player2) {
					game->movePlayer(true, -10, 0);
				}		
				break;
			case ChatMessage::RIGHT:
				if (message.nick == player1) {
					game->movePlayer(false, 10, 0);
				} else if (message.nick == player2) {
					game->movePlayer(true, 10, 0);
				}			
			break;
			case ChatMessage::SHOOT:
				if (message.nick == player1) {
					game->bullets.push_back(std::pair<int16_t, int16_t>(
						game->x1, game->y1 + game->playerRadius + game->bulletRadius)
					);
				} else if (message.nick == player2) {
					game->bullets.push_back(std::pair<int16_t, int16_t>(
						game->x2, game->y2 - game->playerRadius - game->bulletRadius)
					);
				}			
			break;
			case ChatMessage::LOGIN:
				if (client1 == nullptr) {
					client1 = sdMessage;
					player1 = message.nick;
					std::cout << "Player 1 (" << *sdMessage << ") logged in\n";
				} else if (client2 == nullptr) {
					client2 = sdMessage;
					player2 = message.nick;
					std::cout << "Player 2 (" << *sdMessage << ") logged in\n";
				} else {
					std::cout << *sdMessage << " tried to connect but lobby is full\n";
				}
			break;
			case ChatMessage::LOGOUT:
				if(message.nick == player1) {
					client1 = nullptr;
					player1 = "";
					std::cout << "Player 1 (" << *sdMessage << ") logged out\n";
				} else if(message.nick == player2) {
					client2 = nullptr;
					player2 = "";
					std::cout << "Player 2 (" << *sdMessage << ") logged out\n";
				}
			break;
		}
    }
}
void ChatClient::login()
{
    std::string msg;
    ChatMessage em(nick);
    em.type = ChatMessage::LOGIN;
    socket.send(em, socket);
}
void ChatClient::logout()
{
	std::string msg;
    ChatMessage em(nick);
    em.type = ChatMessage::LOGOUT;
    socket.send(em, socket);
}
void ChatClient::input_thread()
{
	char k;
	ChatMessage em(nick);
	do {
        k = dpy->wait_key();
        switch(k) {
			case 'w':
				em.type = ChatMessage::UP;
				socket.send(em, socket);
			break;
			case 'a':
				em.type = ChatMessage::LEFT;
				socket.send(em, socket);
			break;
			case 's':
				em.type = ChatMessage::DOWN;
				socket.send(em, socket);
			break;
			case 'd':
				em.type = ChatMessage::RIGHT;
				socket.send(em, socket);
			break;
			case ' ':
				em.type = ChatMessage::SHOOT;
				socket.send(em, socket);
			break;
			case 'q':
				logout();
				exit = true;
			break;
		}
    } while (!exit);
}
void ChatClient::net_thread()
{
    while(!exit)
    {
		socket.recv(*game);
		dpy->clear();
		// Players
		dpy->set_color(XLDisplay::BLUE);
		dpy->circle(game->x1, game->y1, game->playerRadius);
		dpy->set_color(XLDisplay::GREEN);
		dpy->circle(game->x2, game->y2, game->playerRadius);
		// Middle line
		dpy->set_color(XLDisplay::RED);
		dpy->line(0, 250, 500, 250);
		// Upper and lower limits
		dpy->set_color(XLDisplay::BLACK);
		dpy->line(0, game->upperLimit, 500, game->upperLimit);
		dpy->line(0, game->lowerLimit, 500, game->lowerLimit);
		for (int i = 0; i < game->bullets.size(); i++) {
			dpy->circle(game->bullets[i].first, game->bullets[i].second, game->bulletRadius);
		}
    }
}