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
        alloc_data(SIZE);

        memset(_data, 0, SIZE);

        char* dt = _data;
        
        memcpy(dt, static_cast<void*>(&x1), sizeof(uint8_t));
        dt += sizeof(uint8_t);
        
        memcpy(dt, static_cast<void*>(&y1), sizeof(uint8_t));
        dt += sizeof(uint8_t);

        memcpy(dt, static_cast<void*>(&x2), sizeof(uint8_t));
        dt += sizeof(uint8_t);

        memcpy(dt, static_cast<void*>(&y2), sizeof(uint8_t));

        printf("Posicion x1: %d \n ", x1);
        printf("Posicion y1: %d \n ", y1);
        printf("Posicion x2: %d \n ", x2);
        printf("Posicion y2: %d \n ", y2);
    }

    int from_bin(char * dt) {
        //memcpy(static_cast<void*>(_data), dt, SIZE);
        dt = _data;

        memcpy(static_cast<void*>(&x1), dt, sizeof(uint8_t));
        dt += sizeof(uint8_t);
        
        memcpy(static_cast<void*>(&y1), dt, sizeof(uint8_t));
        dt += sizeof(uint8_t);

        memcpy(static_cast<void*>(&x2), dt, sizeof(uint8_t));
        dt += sizeof(uint8_t);

        memcpy(static_cast<void*>(&y2), dt, sizeof(uint8_t));

        printf("Posicion x1: %d \n ", x1);
        printf("Posicion y1: %d \n ", y1);
        printf("Posicion x2: %d \n ", x2);
        printf("Posicion y2: %d \n ", y2);
        
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
