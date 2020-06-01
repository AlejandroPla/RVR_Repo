#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "Serializable.h"
#include "Socket.h"

class Game : public Serializable {
public:
    static const size_t SIZE = sizeof(uint8_t) * 4;
    uint8_t x1 = -1;
    uint8_t y1 = -1;
    uint8_t x2 = -1;
    uint8_t y2 = -1;
	
    Game(uint8_t _x1, uint8_t _y1, uint8_t _x2, uint8_t _y2) {
        x1 = _x1;
        y1 = _y1;
        x2 = _x2;
        y2 = _y2;
    };
	
    void to_bin() {
        alloc_data(sizeof(int16_t) * 4);
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

    //Game game;
};
