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

// Dice que hay que poner una q para dejar el chat

void ChatServer::do_messages()
{
    while (true)
    {
    //Recibir Mensajes en y en función del tipo de mensaje
    // - LOGIN: Añadir al vector clients
    // - LOGOUT: Eliminar del vector clients
    // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
	ChatMessage message;
	Socket* sdMessage;
	socket.recv(message, sdMessage);
	switch(message.type) {
		case ChatMessage::LOGIN:
		clients.push_back(sdMessage);
		std::cout << "login: " << *sdMessage << "\n";
		break;

		case ChatMessage::MESSAGE:
		for (auto it = clients.begin(); it != clients.end(); ++it) {
					if(!(*(*it) == *sdMessage)) {
						socket.send(message, *(*it));
					}
				}
		std::cout << "message from: " << *sdMessage << "\n";
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
		break;
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

void ChatClient::net_thread()
{
    while(true)
    {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
		ChatMessage em;
		socket.recv(em);		

		std::cout << em.nick << ": " << em.message << "\n";
    }
}


