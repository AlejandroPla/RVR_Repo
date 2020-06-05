#include <unistd.h>
#include <string>
#include <string.h>
#include "ChatMessage.h"
#include "Game.h"
#include "Socket.h"
class Server {
public:
    Server(const char * s, const char * p): socket(s, p) {
        socket.bind();
        game = new Game();
    };
    void do_messages();
    void update_thread();
    Socket socket;
    Game* game;
    Socket* client1 = nullptr;
    Socket* client2 = nullptr;
    std::string player1;
    std::string player2;
};