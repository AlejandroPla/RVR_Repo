#include <string>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "Serializable.h"
#include "Socket.h"
#include "Chat.h"
extern "C" void * _server_thread(void *arg)
{
    ChatServer * server = static_cast<ChatServer *>(arg);
    server->update_thread();
    return 0;
}
int main(int argc, char **argv)
{
    sigset_t waitset;
    int      sig;
    ChatServer es(argv[1], argv[2]);
    pthread_attr_t attr;
    pthread_t id;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&id, &attr, _server_thread, static_cast<void *>(&es));
    es.do_messages();
    sigemptyset(&waitset);
    sigaddset(&waitset, SIGQUIT);
    sigwait(&waitset, &sig);
    return 0;
}