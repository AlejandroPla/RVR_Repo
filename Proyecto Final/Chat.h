#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "Serializable.h"
#include "Socket.h"
#include "XLDisplay.h"
class Game : public Serializable {
public:
    static const size_t SIZE = sizeof(int16_t) * 4;
    int16_t x1 = 250;
    int16_t y1 = 50;
    int16_t x2 = 250;
    int16_t y2 = 450;
    int16_t playerRadius = 20;
    Game() {};
    int movePlayer(bool player, int16_t x, int16_t y) {
        if (!player) {  // Jugador 1
            if (y1 + y + playerRadius >= 250) {
                // El jugador 1 pierde
            } else {
                // TODO: Comprobar y limitar movimiento hacia arriba
                // TODO: Comprobar y limitar movimiento hacia abajo
                // TODO: Comprobar y limitar movimiento hacia la izquierda
                // TODO: Comprobar y limitar movimiento hacia la derecha
            }
        } else {    // Jugador 2
            if (y2 + y - playerRadius <= 250) {
                // El jugador 2 pierde
            } else {
                // TODO: Comprobar y limitar movimiento hacia arriba
                // TODO: Comprobar y limitar movimiento hacia abajo
                // TODO: Comprobar y limitar movimiento hacia la izquierda
                // TODO: Comprobar y limitar movimiento hacia la derecha
            }
        }
    }
    void to_bin() {
        alloc_data(SIZE);
        char* dt = _data;
        memcpy(dt, &x1, sizeof(int16_t));                                          
        dt += sizeof(int16_t); 
        memcpy(dt, &y1, sizeof(int16_t)); 
        dt += sizeof(int16_t); 
        memcpy(dt, &x2, sizeof(int16_t)); 
        dt += sizeof(int16_t); 
        memcpy(dt, &y2, sizeof(int16_t)); 
    }
    int from_bin(char * bobj) {
        memcpy(&x1, bobj, sizeof(int16_t));
        bobj += sizeof(int16_t);
        memcpy(&y1, bobj, sizeof(int16_t));
        bobj += sizeof(int16_t);
        memcpy(&x2, bobj, sizeof(int16_t));
        bobj += sizeof(int16_t);
        memcpy(&y2, bobj, sizeof(int16_t));
        return 0;
    };
};
class ChatMessage: public Serializable
{
public:
static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t);
    enum MessageType
    {
        LOGIN = 0,
        LOGOUT = 1,
        UP = 2,
        DOWN = 3,
        LEFT = 4,
        RIGHT = 5,
        SHOOT = 6
    };
    ChatMessage(){};
    ChatMessage(const std::string& n) : nick(n) {};
    void to_bin();
    int from_bin(char * bobj);
    uint8_t type;
    std::string nick;
};
class ChatServer
{
public:
    ChatServer(const char * s, const char * p): socket(s, p)
    {
        socket.bind();
        game = new Game();
    };
    void do_messages();
    void update_thread() {
        while(true) {
            usleep(10000);
            if (client1 != nullptr) {
                socket.send(*game, *client1);
            }
            if (client2 != nullptr) {
                socket.send(*game, *client2);
            }
        }
    }
private:
    Socket socket;
    Game* game;
    Socket* client1 = nullptr;
    Socket* client2 = nullptr;
    std::string player1;
    std::string player2;
};
class ChatClient
{
public:
    ChatClient(const char * s, const char * p, const char * n)
    : socket(s, p), nick(n) {
        game = new Game();
        dpy = &XLDisplay::display();
    };
    void login();
    void logout();
    void input_thread();
    void net_thread();
private:
    Socket socket;
    std::string nick;
    Game* game;
    XLDisplay* dpy;
    bool exit = false;
};