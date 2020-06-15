#include "Game.h"
#include "Socket.h"

class Server {
protected:
    Socket socket;
    Socket* client1 = nullptr;
    Socket* client2 = nullptr;

    Game* game;
    std::string name_player1;
    std::string name_player2;

public:
    Server(const char * s, const char * p): socket(s, p) {
        socket.bind();
        game = new Game();
    };

    void do_messages();
    void update_thread();
    void updateBullets();
};