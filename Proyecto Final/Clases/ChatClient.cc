#include <string>
#include <unistd.h>
#include <string.h>
#include "Client.h"
#include "XLDisplay.h"
extern "C" void * _client_thread(void *arg) {
    Client * server = static_cast<Client *>(arg);
    server->net_thread();
    return 0;
}
int main(int argc, char **argv) {
    Client ec(argv[1], argv[2], argv[3]);
    pthread_attr_t attr;
    pthread_t id;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&id, &attr, _client_thread, static_cast<void *>(&ec));
    XLDisplay::init(500, 500, "Drone Duel");
    ec.login();
    ec.input_thread();
}