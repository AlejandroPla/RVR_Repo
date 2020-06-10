#include <string>
#include <string.h>
class Player {
public:
    // variables
    int16_t pos_x;
    int16_t pos_y;
    int16_t lives = 3;
    // metodos
    Player(int16_t x, int16_t y);
};