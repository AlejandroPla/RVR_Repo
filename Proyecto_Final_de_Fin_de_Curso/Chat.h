#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "Serializable.h"
#include "Socket.h"
#include "XLDisplay.h"

class ChatMessage: public Serializable
{
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t);

    enum MessageType
	{
        LOGIN   = 0,
        MESSAGE = 1,
        LOGOUT  = 2
    };

    ChatMessage(){};

    ChatMessage(const std::string& n, const std::string& m):nick(n),message(m){};

    void to_bin();

    int from_bin(char * bobj);

    uint8_t type;

    std::string nick;
    std::string message;
};

class ChatServer
{
public:
    ChatServer(const char * s, const char * p): socket(s, p)
    {
        socket.bind();
    };

    /**
     *  Thread principal del servidor recive mensajes en el socket y
     *  lo distribuye a los clientes. Mantiene actualizada la lista de clientes
     */
    void do_messages();

private:
    /**
     *  Lista de clientes conectados al servidor de Chat, representados por
     *  su socket
     */
    std::vector<Socket *> clients;

	/**
     * Socket del servidor
     */
    Socket socket;
};

/**
 *  Clase para el cliente de chat
 */
class ChatClient
{
public:
    /**
     * @param s dirección del servidor
     * @param p puerto del servidor
     * @param n nick del usuario
     */
	ChatClient(const char * s, const char * p, const char * n):socket(s, p),
        nick(n){};

    /**
     *  Envía el mensaje de login al servidor
     */
    void login();

    /**
     *  Envía el mensaje de logout al servidor
     */
    void logout();

     /**
     *  Rutina principal para el Thread de E/S. Lee datos de STDIN (std::getline)
	 *  y los envía por red vía el Socket.
     */
    void input_thread();

    /**
     *  Rutina del thread de Red. Recibe datos de la red y los "renderiza"
     *  en STDOUT
     */
    void net_thread();

    // Para el renderizado
    void draw_character(uint8_t x, uint8_t y, uint8_t z);

    void update_position(char c);

    uint8_t get_life() { return life; }

private:

    /**
     * Socket para comunicar con el servidor
     */
 	Socket socket;

    /**
     * Nick del usuario
     */
    std::string nick;

    // Vida del usuario
    uint8_t life = 50;
};

/**
 *  Clase para el renderizado del cliente
 */
/*class RenderClient : public XLDisplay {
public:
    RenderClient()
    {
        XLDisplay::init(600, 800, "Ejemplo");
    };
    
    
    void renderUpdate();

    void setPosition(uint8_t x_, uint8_t y_, uint8_t z_);

    XLDisplay* getDisplay() { return dpy; };

    uint8_t getX() { return x; };
    uint8_t getY() { return y; };
    uint8_t getZ() { return z; };

private:
    XLDisplay* dpy;

    uint8_t x;
    uint8_t y;
    uint8_t z;
};*/
