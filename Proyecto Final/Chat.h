#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "Serializable.h"
#include "Socket.h"
#include "XLDisplay.h"

class Player {
public:
    int x;
    int y;
    int size = 50;

    Player(int n) {
        if (n = 1) {
            x = 250;
            y = 50;
        }

        if (n = 2) {
            x = 250;
            y = 450;
        }
    }
};

class Bullet {
    Bullet(){}

    int x;
    int y;
};

class Game: public Serializable
{
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t);

    Game() {
        player1 = new Player(1);
        player2 = new Player(2);
    };

    void to_bin();

    int from_bin(char * data);

    void draw();

    int getSize() { return _size; }

    Player* player1;
    Player* player2;

    std::vector<Bullet*> bullets;
};

class ChatMessage: public Serializable
{
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t);

    enum MessageType
	{
        LOGIN   = 0,
        MOVE_LEFT = 1,
        MOVE_RIGHT = 2,
        MOVE_UP = 3,
        MOVE_DOWN= 4,
        SHOOT = 5,
        LOGOUT  = 6
    };

    ChatMessage(){};

    ChatMessage(const std::string& n, const std::string& m):nick(n),message(m){};

    void to_bin() {}

    int from_bin(char * bobj) {}

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
        game = Game();
    };

    void do_messages();

    void update_clients();

private:
    std::vector<Socket *> clients;
    Socket socket;
    
    int nPlayers = 0;

    Game game;
};

class ChatClient {
public:
	ChatClient(const char * s, const char * p, const char * n):socket(s, p),
        nick(n)
        {
            game = Game();
        };

    void login();

    void logout();

    void input_thread();

    void net_thread();

    void draw_character();

    void handleInput();

    void update();

private:
    Game game;

 	Socket socket;
    std::string nick;
};
