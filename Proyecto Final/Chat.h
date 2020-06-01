#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "Serializable.h"
#include "Socket.h"
class Game : public Serializable {
public:
    static const size_t SIZE = sizeof(int16_t) * 4;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
    Game(int16_t _x1, int16_t _y1, int16_t _x2, int16_t _y2) {
        x1 = _x1;
        y1 = _y1;
        x2 = _x2;
        y2 = _y2;
    };
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
        LOGIN   = 0,
        MESSAGE = 1,
        LOGOUT  = 2,
        UP  = 3,
        DOWN  = 4,
        LEFT  = 5,
        RIGHT  = 6,
        SHOOT  = 7
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
        game = new Game(250, 50, 250, 450);
    };
    void do_messages();
    void update_thread() {
        while(true) {
            if (client1 != nullptr) {
                socket.send(*game, *client1);
            }
            if (client2 != nullptr) {
                socket.send(*game, *client2);
            }
        }
    }
private:
    std::vector<Socket *> clients;
    Socket* client1 = nullptr;
    Socket* client2 = nullptr;
    Socket socket;
    std::string player1;
    std::string player2;
    Game* game;
};
class ChatClient
{
public:
    ChatClient(const char * s, const char * p, const char * n):socket(s, p),
        nick(n){};
    void login();
    void logout();
    void input_thread();
    void net_thread();
    private:
    Socket socket;
    std::string nick;
};