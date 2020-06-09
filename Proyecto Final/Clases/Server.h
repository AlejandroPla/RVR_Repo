#include <unistd.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "ChatMessage.h"
#include "Game.h"
#include "Socket.h"
class Server {
public:
    Socket socket;
    Game* game;
    Socket* client1 = nullptr;
    Socket* client2 = nullptr;
    std::string player1;
    std::string player2;
    //struct timespec time1, time2;
    Server(const char * s, const char * p): socket(s, p) {
        socket.bind();
        game = new Game();
		//time1.tv_sec = 0;
        //time1.tv_nsec = 5000000L;
    };
    void do_messages();
    void update_thread();
    void check_collisions();
};