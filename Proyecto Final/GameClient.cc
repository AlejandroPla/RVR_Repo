#include "Client.h"
#include "XLDisplay.h"

extern "C" void * _input_thread(void *arg) {
    Client * server = static_cast<Client *>(arg);
    server->input_thread();
    return 0;
}

extern "C" void * _render_thread(void *arg) {
    Client * server = static_cast<Client *>(arg);
    server->render_thread();
    return 0;
}

int main(int argc, char **argv) {
    Client ec(argv[1], argv[2], argv[3]);

    pthread_attr_t attr;
    pthread_t id;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&id, &attr, _input_thread, static_cast<void *>(&ec));

    pthread_attr_t attr2;
    pthread_t id2;
    pthread_attr_init(&attr2);
    pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_DETACHED);
    pthread_create(&id2, &attr2, _render_thread, static_cast<void *>(&ec));

    XLDisplay::init(500, 500, "Drone Duel");

    ec.login();
    ec.net_thread();
}