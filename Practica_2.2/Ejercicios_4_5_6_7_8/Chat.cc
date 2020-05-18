#include "Chat.h"

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
	memcpy(_data, &type, sizeof(type));
	_data += sizeof(type);
	memcpy(_data, &nick, sizeof(nick));
	_data += sizeof(nick);
	memcpy(_data, &message, sizeof(message));
	_data += sizeof(message);
}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    //Reconstruir la clase usando el buffer _data
	memcpy(&type, _data, sizeof(type));
	_data += sizeof(type);
	memcpy(&nick, _data, sizeof(nick));
	_data += sizeof(nick);
	memcpy(&message, _data, sizeof(message));
	_data += sizeof(message);

    return 0;
}

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
			case 0:
			clients.push_back(sdMessage);
			break;

			case 1:
			for (Socket* s = clients.begin(); s != clients.end(); s++) {
				if (s != sdMessage) {
					socket.send(message, *s);
				}
			}
			break;

			case 2:
			int i;
			i = 0;
			bool encontrado;
			encontrado = false;
			
			while (!encontrado && i != clients.size()) {
				if (clients[i] == sdMessage) {
					encontrado = true;
					clients.erase(clients.begin() + i);
				}
			}
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

		ChatMessage em(nick, msg);
		em.type = ChatMessage::MESSAGE;
		
		socket.send(em, socket);
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

