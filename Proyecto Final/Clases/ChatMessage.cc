#include "ChatMessage.h"
void ChatMessage::to_bin() {
    alloc_data(MESSAGE_SIZE);
    memset(_data, 0, MESSAGE_SIZE);
	char* dt = _data;
    memcpy(dt, &type, sizeof(uint8_t));
	dt += sizeof(uint8_t);
	for (unsigned int i = 0; i < 7 && i < nick.length(); i++, dt++) {
		*dt = nick[i];
	}
}
int ChatMessage::from_bin(char * bobj) {
    alloc_data(MESSAGE_SIZE);
    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);
	type = (uint8_t) *_data;
	char * _nick = _data + sizeof(uint8_t);
	std::string n(_nick, 8);
	nick = n;
    return 0;
}