#include <string>
#include <string.h>
#include "Serializable.h"
#include "Socket.h"
class ChatMessage: public Serializable {
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t);
    enum MessageType {
        LOGIN = 0,
        LOGOUT = 1,
        UP = 2,
        DOWN = 3,
        LEFT = 4,
        RIGHT = 5,
        SHOOT = 6
    };
	ChatMessage(){};
    ChatMessage(const std::string& n) : nick(n) {};
    void to_bin();
    int from_bin(char * bobj);
    uint8_t type;
    std::string nick;
};