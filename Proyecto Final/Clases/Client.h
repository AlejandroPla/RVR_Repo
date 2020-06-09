#include <string>
#include <string.h>
#include <unistd.h>
#include "ChatMessage.h"
#include "Game.h"
#include "Serializable.h"
#include "Socket.h"
#include "XLDisplay.h"
class Client {
public:
    Client(const char * s, const char * p, const char * n) : socket(s, p), nick(n) {
        game = new Game();
        dpy = &XLDisplay::display();
    };
    void login();
    void logout();
    void input_thread();
    void net_thread();
    Socket socket;
    std::string nick;
    Game* game;
    XLDisplay* dpy;
    bool exit = false;
};