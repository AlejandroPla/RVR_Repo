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
    
    Player(int16_t _n, int16_t _x, int16_t _y, int16_t _r);

    virtual void render();
};

class Bullet : public GameObject {
public:
    int16_t speed;
    int16_t radius;
    int16_t player;
    XLDisplay* dpy;

    Bullet(int16_t _x, int16_t _y, int16_t _player, int16_t _r, int16_t _s);

    void update();

    bool check_collision(int16_t border_up, int16_t border_down);

    virtual void render();
};