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
			login();
			break;

			case 1:
			for (auto s : clients) {
				if (s != sdMessage) {
					socket.send(message, s);
				}
			}
			break;

			case 2:
			logout(); // al parecer el metodo esta vacio
			break;
			
			default:
			printf("Invalid message type\n");
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
	
}

void ChatClient::input_thread()
{
    while (true)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
    }
}

void ChatClient::net_thread()
{
    while(true)
    {		
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
		printf("%s: %s\n", nick, message);
    }
}

