#include "Chat.h"

#include <algorithm>
#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "XLDisplay.h"

// Game methods
void Game::to_bin(){
	alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

	char* dt = _data;

	memcpy(dt, &player1->x, sizeof(player1->x));
	dt += sizeof(player1->x);

	memcpy(dt, &player1->y, sizeof(player1->y));
	dt += sizeof(player1->y);

	memcpy(dt, &player2->x, sizeof(player2->x));
	dt += sizeof(player2->x);

	memcpy(dt, &player2->y, sizeof(player2->y));
	dt += sizeof(player2->y);

	/*for (int i = 0; i < bullet.size(); i++) {
		//memcpy(dt, &)
	}*/

	int fileWrite = open("Game_data", O_CREAT | O_WRONLY);
    write(fileWrite, data(), size());
    close(fileWrite);
}

int Game::from_bin(char * data) {
	alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), data, MESSAGE_SIZE);
	
	player1->x = *_data + sizeof(int);
	player1->y = *_data + sizeof(int);

	player2->x = *_data + sizeof(int);
	player2->y = *_data + sizeof(int);

    return 0;
}

void Game::draw() {
	XLDisplay& dpy = XLDisplay::display();

	// Linea central
    dpy.set_color(XLDisplay::RED);
    dpy.rectangle(250, 250, 500, 10);

    dpy.set_color(XLDisplay::BLUE);
    dpy.circle(player1->x,player1->y, player1->size);

    dpy.set_color(XLDisplay::GREEN);
	dpy.circle(player2->x,player2->y, player2->size);

	// TODO balas y lineas
}

// ChatServer methods
void ChatServer::do_messages() {
    while (true)
    {
	ChatMessage message;
	Socket* sdMessage;
	socket.recv(message, sdMessage);
	auto it = clients.begin();

	switch(message.type) {
		case ChatMessage::LOGIN:
			if (nPlayers < 2) {
				nPlayers++;
				clients.push_back(sdMessage);
				std::cout << "login: " << *sdMessage << "\n";
			} else {
				// TODO Enviar mensaje de que la sala esta llena
				/*message.message = "lobby is full\n";

				socket.send(message, *sdMessage);
				std::cout << *sdMessage << " tried to join but lobby is full\n";*/
			}
		break;

		case ChatMessage::MOVE_LEFT:
			if (*(*it) == *sdMessage) {
				game.player1->x -= 10;
			} else {
				game.player2->x -= 10;
			}
		break;

		case ChatMessage::MOVE_RIGHT:
			if (*(*it) == *sdMessage) {
				game.player1->x += 10;
			} else {
				game.player2->x += 10;
			}
		break;

		case ChatMessage::MOVE_UP:
			if (*(*it) == *sdMessage) {
				game.player1->y -= 10;
			} else {
				game.player2->y -= 10;
			}
		break;
		
		case ChatMessage::MOVE_DOWN:
			if (*(*it) == *sdMessage) {
				game.player1->y += 10;
			} else {
				game.player2->y += 10;
			}
		break;

		case ChatMessage::LOGOUT:
			int i;
			i = 0;
			bool encontrado;
			encontrado = false;

			while (!encontrado && i != clients.size()) {
				if (clients[i] == sdMessage) {
					encontrado = true;
					clients.erase(clients.begin() + i);
				}
				else i++;
			}

			std::cout << "logout: " << *sdMessage << "\n";
			nPlayers--;
		break;
		}
	}
}

void ChatServer::update_clients()
{
	while(true) {
		game.to_bin();
		for (auto it = clients.begin(); it != clients.end(); ++it) {
			//std::cout << "ENTRA A UPDATE CLIENTS\n";
			socket.send(game, *(*it));
		}
	}
}

void ChatClient::login()
{
	std::string msg;

	ChatMessage em(nick, msg);
	em.type = ChatMessage::LOGIN;

	socket.send(em, socket);
}

void ChatClient::logout()
{
	std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGOUT;

    socket.send(em, socket);
}

void ChatClient::input_thread()
{
    /*while (true)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
		std::string msg;
		std::getline(std::cin, msg);

		if (msg == "q") {
			ChatMessage em(nick, msg);
			em.type = ChatMessage::LOGOUT;
		
			socket.send(em, socket);
			break;
		}
		else {
			ChatMessage em(nick, msg);
			em.type = ChatMessage::MESSAGE;
			
			socket.send(em, socket);
		}
    }*/
}

void ChatClient::net_thread()
{
    while(true)
    {
		ChatMessage em;
		socket.recv(em);		

		std::cout << em.nick << ": " << em.message << "\n";
    }
}

void ChatClient::update()
{
	while (true) {
		socket.recv(game);
		
		char buffer[game.getSize()];

		int fileRead = open("Game_data", O_RDONLY);
		
		read(fileRead, buffer, game.getSize());
		close(fileRead);

		game.from_bin(buffer);

		XLDisplay& dpy = XLDisplay::display();
		dpy.clear();

		game.draw();
	}
}

void ChatClient::handleInput()
{
	XLDisplay& dpy = XLDisplay::display();
    char k;

	do {
        k = dpy.wait_key();
        switch(k) {
			case 'w':
				// TODO Enviar mensaje MOVE_UP
			break;
			case 'a':
				// TODO Enviar mensaje MOVE_LEFT
			break;
			case 's':
				// TODO Enviar mensaje MOVE_DOWN
			break;
			case 'd':
				// TODO Enviar mensaje MOVE_RIGHT
			break;
			case ' ':
				// TODO Enviar mensaje SHOOT
			break;
		}
    } while (k != 'q');
}
