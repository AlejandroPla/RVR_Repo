#include "Chat.h"
#include "XLDisplay.h"
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

	dt += 7 * sizeof(char);

	for (unsigned int i = 0; i < message.length(); i++, dt++) {
	 *dt = message[i];
	}

	dt = _data + sizeof(uint8_t) + 8* sizeof(char);
}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

	type = (uint8_t) *_data;

	char * _nick = _data + sizeof(uint8_t);
	char * _msg = _nick + sizeof(char) * 8;

	std::string n(_nick, 8);
	std::string m(_msg, 80);
	
	message = m;
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
	auto it = clients.begin();

	switch(message.type) {
		case ChatMessage::LOGIN:
			if (nPlayers < 2) {
				nPlayers++;
				clients.push_back(sdMessage);
				std::cout << "login: " << *sdMessage << "\n";
			} else {
				// TODO Enviar mensaje de que la sala esta llena
				std::cout << *sdMessage << " tried to join but lobby is full\n";
			}
		break;
		/*case ChatMessage::UPDATE:
			for (auto it = clients.begin(); it != clients.end(); ++it) {
				if(!(*(*it) == *sdMessage)) {
					socket.send(message, *(*it));
				}
			}
			std::cout << "message from: " << *sdMessage << "\n";
		break;*/
		case ChatMessage::MOVE_LEFT:
			if (*(*it) == *sdMessage) {
				player1->x -= 10;
			} else {
				player2->x -= 10;
			}
		break;
		case ChatMessage::MOVE_RIGHT:
			if (*(*it) == *sdMessage) {
				player1->x += 10;
			} else {
				player2->x += 10;
			}
		break;
		case ChatMessage::MOVE_UP:
			if (*(*it) == *sdMessage) {
				player1->y -= 10;
			} else {
				player2->y -= 10;
			}
		break;
		case ChatMessage::MOVE_DOWN:
			if (*(*it) == *sdMessage) {
				player1->y += 10;
			} else {
				player2->y += 10;
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

void ChatServer::update_clients() {
	// TODO: Serializar y enviar Game a los clientes 
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
/*
void ChatClient::input_thread()
{
    while (true)
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
    }
}
*/

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
	// TODO esperar a recibir algo del servidor (recv)
	Game game;
	socket.recv(game);
	
	// TODO Deserializar el paquete

	// TODO Usar esos datos para repintar todo
	XLDisplay& dpy = XLDisplay::display();
	dpy.clear();

	//XLDisplay& dpy = XLDisplay::display();
	//dpy.set_color(XLDisplay::RED);
	//dpy.circle(x, y, size);
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

