#include "Game.h"
#include "Socket.h"
#include "XLDisplay.h"

class Client {
protected:
    Socket socket;
    std::string nick;

    Game* game;
    XLDisplay* dpy;
    
    bool exit = false;

public:
    Client(const char * s, const char * p, const char * n) : socket(s, p), nick(n) {
        game = new Game();
        dpy = &XLDisplay::display();
    };
    
    void login();
    void logout();

    void input_thread();
    void net_thread();
    void render_thread();
};