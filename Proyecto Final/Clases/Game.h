#include <vector>
#include "Bullet.h"
#include "Serializable.h"

class Player {
public:
    int16_t pos_x;
    int16_t pos_y;
    int16_t lives = 3;
    
    Player(int16_t x, int16_t y) {
        pos_x = x;
        pos_y = y;
    }
};

class Game : public Serializable {
public:
    // Variables
    static const size_t SIZE = sizeof(int16_t) * 8 + sizeof(Player) * 2;
    Player* player1;
    Player* player2;
    int16_t upperLimit = 5;
    int16_t lowerLimit = 495;
    int16_t playerRadius = 20;
    int16_t bulletRadius = 5;
    std::vector<Bullet> bullets;
    std::string winning_player;
    // Metodos
    Game() {
        player1 = new Player(250, 50);
        player2 = new Player(250, 450);
    };
	void movePlayer(bool player, int16_t x, int16_t y);
	void playerHit(bool player);
	int bullet_collides_player(Bullet bull);
	void to_bin();
	int from_bin(char* bobj);
    bool game_over();
    void reset();
};