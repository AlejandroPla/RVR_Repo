#include <string>
#include <string.h>
#include <unistd.h>
#include <cmath>
#include "XLDisplay.h"

class GameObject {
public:
    int16_t x;
    int16_t y;

    GameObject(int16_t _x, int16_t _y) : x(_x), y(_y) {}

    virtual void render() = 0;
};

class Player : public GameObject {
public:
    int16_t n;
    int16_t radius;
    int16_t lives = 3;
    XLDisplay* dpy;
    
    Player(int16_t _n, int16_t _x, int16_t _y, int16_t _r) : GameObject(_x, _y), n(_n), radius(_r) {
        dpy = &XLDisplay::display();
    }

    virtual void render() {
        if (n) {
            dpy->set_color(XLDisplay::GREEN);
        } else {
            dpy->set_color(XLDisplay::BLUE);
        }
		dpy->circle(x, y, radius);
    }
};

class Bullet : public GameObject {
public:
    int16_t bulletRadius = 5;
    int16_t bulletSpeed = 1;
    int16_t radius;
    int16_t player; // 0 es player1 / 1 es player2
    XLDisplay* dpy;

    Bullet(int16_t _x, int16_t _y, int16_t _player, int16_t _r) : GameObject(_x, _y) {
        dpy = &XLDisplay::display();
        player = _player;
        radius = _r;
    }

    void update() {
        if (!player) {
            y += bulletSpeed;
        } else {
            y -= bulletSpeed;
        }
    }

    bool check_collision(int16_t border_up, int16_t border_down) {
        if (y <= border_up || y >= border_down) {
            return true;
        }
        return false;
    }

    virtual void render() {
        dpy->circle(x, y, radius);
    }
};