#include <string>
#include <string.h>
#include <unistd.h>
#include <cmath>
class Bullet {
public:
    Bullet(int16_t x, int16_t y, int16_t player_) {
        bullet_x = x;
        bullet_y = y;
        player = player_;
    }
    void update();
    bool check_collision(int16_t border_up, int16_t border_down);
    int16_t bullet_x;
    int16_t bullet_y;
    int16_t bulletRadius = 5;
    int16_t bulletSpeed = 1;
    int16_t player; // 0 es player1 / 1 es player2
};