#include "Bullet.h"
void Bullet::update() {
    if (!player) {
        bullet_y += bulletSpeed;
    } else {
        bullet_y -= bulletSpeed;
    }
}
bool Bullet::check_collision(int16_t border_up, int16_t border_down) {
    if (bullet_y <= border_up || bullet_y >= border_down) {
        return true;
    }
    return false;
}