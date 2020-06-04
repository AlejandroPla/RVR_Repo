#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <cmath>
#include "Serializable.h"
#include "Socket.h"
#include "XLDisplay.h"
class Bullet {
public:
    int16_t bullet_x;
    int16_t bullet_y;
    int16_t bulletRadius = 5;
    int16_t bulletSpeed = 1;
    int16_t player; // 0 es player1 / 1 es player2
    Bullet(int16_t x, int16_t y, int16_t player_) {
        bullet_x = x;
        bullet_y = y;
        player = player_;
    }
    void update() {
        if (!player) {
            bullet_y += bulletSpeed;
        } else {
            bullet_y -= bulletSpeed;
        }
    }
    bool check_collision(int16_t border_up, int16_t border_down) {
        if (bullet_y <= border_up || bullet_y >= border_down) {
            return true;
        }
        return false;
    }
};
class Game : public Serializable {
public:
    static const size_t SIZE = sizeof(int16_t) * 7;
    int16_t x1 = 250;
    int16_t y1 = 50;
    int16_t x2 = 250;
    int16_t y2 = 450;
    int16_t upperLimit = 5;
    int16_t lowerLimit = 495;
    int16_t playerRadius = 20;
    int16_t bulletRadius = 5;
    std::vector<Bullet> bullets;
    Game() {};
    void movePlayer(bool player, int16_t x, int16_t y) {
        if (!player) {  // Jugador 1
            if (x1 + x + playerRadius <= 500 && x1 + x - playerRadius >= 0) {
                x1 += x;
            }
            if (y1 + y - playerRadius >= upperLimit) {
                y1 += y;
            }
        } else { // Jugador 2
            if (x2 + x + playerRadius <= 500 && x2 + x - playerRadius >= 0) {
                x2 += x;
            }
            if (y2 + y + playerRadius <= lowerLimit) {
                y2 += y;
            }
        }
    }
    void playerHit(bool player) {
        if (!player) {
            upperLimit += 50;
            
            // El limite empuja al jugador
            if (y1 - playerRadius < upperLimit) {
                y1 = upperLimit + playerRadius;
                // El jugador pasa la linea del centro
                if (y1 + playerRadius >= 250) {
                    std::cout << "PIERDE EL JUGADOR 1\n";
                }
            }
        } else {
            lowerLimit -= 50;
            // El limite empuja al jugador
            if (y2 + playerRadius > lowerLimit) {
                y2 = lowerLimit - playerRadius;
                // El jugador pasa la linea del centro
                if (y2 - playerRadius <= 250) {
                    std::cout << "PIERDE EL JUGADOR 2\n";
                }
            }
        }
    }
    int bullet_collides_player(Bullet bull) {
        // Choca con el jugador 1
        int16_t dist = std::sqrt(
            std::abs(bull.bullet_x - x1) * std::abs(bull.bullet_x - x1) +
            std::abs(bull.bullet_y - y1) * std::abs(bull.bullet_y - y1)
        );
        if (dist < playerRadius + bulletRadius) { return 0; }
        // Choca con el jugador 2
        dist = std::sqrt(
            std::abs(bull.bullet_x - x2) * std::abs(bull.bullet_x - x2) +
            std::abs(bull.bullet_y - y2) * std::abs(bull.bullet_y - y2)
        );
        if (dist < playerRadius + bulletRadius) { return 1; }
        // No choca
        return -1;
    }
    void to_bin() {
        // Reserva el tamaño de 7 enteros + el del array de bullets
        size_t bulletsSize = bullets.size() * sizeof(int16_t) * 2;
        alloc_data(SIZE + bulletsSize);
        
        char* dt = _data;
        memcpy(dt, &x1, sizeof(int16_t));                                          
        dt += sizeof(int16_t); 
        memcpy(dt, &y1, sizeof(int16_t)); 
        dt += sizeof(int16_t); 
        memcpy(dt, &x2, sizeof(int16_t)); 
        dt += sizeof(int16_t); 
        memcpy(dt, &y2, sizeof(int16_t));
        dt += sizeof(int16_t); 
        memcpy(dt, &upperLimit, sizeof(int16_t)); 
        dt += sizeof(int16_t); 
        memcpy(dt, &lowerLimit, sizeof(int16_t));
        dt += sizeof(int16_t);
        
        int16_t s = bullets.size();
        memcpy(dt, &s, sizeof(int16_t));
        for (int i = 0; i < bullets.size(); i++) {
            dt += sizeof(int16_t); 
            memcpy(dt, &bullets[i].bullet_x, sizeof(int16_t));
            dt += sizeof(int16_t); 
            memcpy(dt, &bullets[i].bullet_y, sizeof(int16_t));
        }
    }
    int from_bin(char * bobj) {
        memcpy(&x1, bobj, sizeof(int16_t));
        bobj += sizeof(int16_t);
        memcpy(&y1, bobj, sizeof(int16_t));
        bobj += sizeof(int16_t);
        memcpy(&x2, bobj, sizeof(int16_t));
        bobj += sizeof(int16_t);
        memcpy(&y2, bobj, sizeof(int16_t));
        bobj += sizeof(int16_t);
        memcpy(&upperLimit, bobj, sizeof(int16_t));
        bobj += sizeof(int16_t);
        memcpy(&lowerLimit, bobj, sizeof(int16_t));
        int16_t s;
        bobj += sizeof(int16_t);
        memcpy(&s, bobj, sizeof(int16_t));
        bullets.clear();
        for (int i = 0; i < s; i++) {
            int16_t x;
            int16_t y;
            bobj += sizeof(int16_t); 
            memcpy(&x, bobj, sizeof(int16_t));
            bobj += sizeof(int16_t); 
            memcpy(&y, bobj, sizeof(int16_t));
            Bullet bull(x, y, 0);
            bullets.push_back(bull);
        }
        return 0;
    };
};
class ChatMessage: public Serializable
{
public:
static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t);
    enum MessageType
    {
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
class ChatServer
{
public:
    ChatServer(const char * s, const char * p): socket(s, p)
    {
        socket.bind();
        game = new Game();
    };
    void do_messages();
    void update_thread() {
        while(true) {
            usleep(10000);
            
            for (int i = 0; i < game->bullets.size(); i++) {
                game->bullets[i].update();
                if (game->bullets[i].check_collision(game->upperLimit, game->lowerLimit)) {
                    game->bullets.erase(game->bullets.begin() + i);
                }
                if (game->bullet_collides_player(game->bullets[i]) == 0) {
                    game->bullets.erase(game->bullets.begin() + i);
                    game->playerHit(0);
                }
                if (game->bullet_collides_player(game->bullets[i]) == 1) {
                    game->bullets.erase(game->bullets.begin() + i);
                    game->playerHit(1);
                }
            }
            if (client1 != nullptr) {
                socket.send(*game, *client1);
            }
            if (client2 != nullptr) {
                socket.send(*game, *client2);
            }
        }
    }
private:
    Socket socket;
    Game* game;
    Socket* client1 = nullptr;
    Socket* client2 = nullptr;
    std::string player1;
    std::string player2;
};
class ChatClient
{
public:
    ChatClient(const char * s, const char * p, const char * n)
    : socket(s, p), nick(n) {
        game = new Game();
        dpy = &XLDisplay::display();
    };
    void login();
    void logout();
    void input_thread();
    void net_thread();
private:
    Socket socket;
    std::string nick;
    Game* game;
    XLDisplay* dpy;
    bool exit = false;
};