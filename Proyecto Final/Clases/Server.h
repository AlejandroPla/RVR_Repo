#include <unistd.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include "ChatMessage.h"
#include "Game.h"
#include "Socket.h"
class Server {
public:
    // variables
    Socket socket;
    Game* game;
    Socket* client1 = nullptr;
    Socket* client2 = nullptr;
    std::string name_player1;
    std::string name_player2;
    // metodos
    Server(const char * s, const char * p): socket(s, p) {
        socket.bind();
        game = new Game();
    };
    void do_messages();
    void update_thread();
    void check_collisions();
};
