#include <string>

#include <unistd.h>

#include <string.h>

#include "XLDisplay.h"

#include "Chat.h"



extern "C" void * draw_thread(void *arg)

{

    ChatClient * server = static_cast<ChatClient *>(arg);



    server->draw_character();



    return 0;

}



extern "C" void * input_thread(void *arg)

{

    ChatClient * server = static_cast<ChatClient *>(arg);



    server->handleInput();



    return 0;

}



int main(int argc, char **argv)

{

    ChatClient ec(argv[1], argv[2], argv[3]);



    pthread_attr_t attr;

    pthread_t id;



    pthread_attr_init(&attr);

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);



    XLDisplay::init(500, 500, "Drone Duel");



    ec.login();

    pthread_create(&id, &attr, draw_thread, static_cast<void *>(&ec));

    //pthread_create(&id, &attr, input_thread, static_cast<void *>(&ec));



    ec.handleInput();

}



