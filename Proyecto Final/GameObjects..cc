#include "GameObjects.h"

Player::Player(int16_t _n, int16_t _x, int16_t _y, int16_t _r)
    : GameObject(_x, _y), n(_n), radius(_r) {
    dpy = &XLDisplay::display();
}

void Player::render() {
    if (n) {
        dpy->set_color(XLDisplay::GREEN);
    } else {
        dpy->set_color(XLDisplay::BLUE);
    }
    dpy->circle(x, y, radius);
}

Bullet::Bullet(int16_t _x, int16_t _y, int16_t _player, int16_t _r, int16_t _s)
    : GameObject(_x, _y) {
    dpy = &XLDisplay::display();
    player = _player;
    radius = _r;
    speed = _s;
}

void Bullet::update() {
    if (!player) {
        y += speed;
    } else {
        y -= speed;
    }
}

bool Bullet::check_collision(int16_t border_up, int16_t border_down) {
    if (y <= border_up || y >= border_down) {
        return true;
    }
    return false;
}

void Bullet::render() {
    dpy->circle(x, y, radius);
}