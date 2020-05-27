#include <string>
#include <unistd.h>
#include <string.h>
#include "Chat.h"

#include "XLDisplay.h"

#include <string>

extern "C" void * _client_thread(void *arg)
{
    ChatClient * server = static_cast<ChatClient *>(arg);

    server->net_thread();

    return 0;
}

int main(int argc, char **argv)
{
    ChatClient ec(argv[1], argv[2], argv[3]);

    pthread_attr_t attr;
    pthread_t id;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&id, &attr, _client_thread, static_cast<void *>(&ec));

    ec.login();

    //ec.input_thread();

    std::cout << "ENTRA AL RENDERIZADO\n";

    // Renderizado
    XLDisplay::init(600, 800, "Jugador1");

    XLDisplay& dpy = XLDisplay::display();

    ec.draw_character(45, 45, 15);

    char k;

    do {
        k = dpy.wait_key();
        ec.update_position(k);
    } while (ec.get_life() > 0 || k != 'q');

    dpy.clear();

    dpy.flush();

    sleep(1);
}
