#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "Serializable.h"
#include "Socket.h"

class Game : public Serializable {
public:
    static const size_t SIZE = sizeof(uint8_t) * 4;
    uint8_t x1;
    uint8_t y1;
    int x2;
    int y2;
	
    Game(uint8_t _x1, uint8_t _y1, uint8_t _x2, uint8_t _y2) {
        x1 = _x1;
        y1 = _y1;
        x2 = _x2;
        y2 = _y2;
        std::cout << x1 << "\n";
    };
	
    void to_bin() {
        alloc_data(sizeof(uint8_t));
        memset(_data, 0, sizeof(uint8_t));
        char* dt = _data;
        memcpy(dt, &x1, sizeof(uint8_t));
        dt += sizeof(uint8_t);
        dt = _data + (sizeof(uint8_t));
    }
	
    int from_bin(char * bobj) {
        alloc_data(sizeof(uint8_t));
        std::cout << x1 << "\n";
        memcpy(static_cast<void *>(_data), bobj, sizeof(uint8_t));
        x1 = (uint8_t) *_data;
        std::cout << x1 << "\n";
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
        game = new Game(200, 50, 125, 200);
    };
	
    void do_messages();
	
private:
    std::vector<Socket *> clients;
    Socket socket;
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