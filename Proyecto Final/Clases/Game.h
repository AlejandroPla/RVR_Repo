#include <vector>
#include "Bullet.h"
#include "Serializable.h"
class Game : public Serializable {
public:
    // Metodos
    Game() {};
	void movePlayer(bool player, int16_t x, int16_t y);
	void playerHit(bool player);
	int bullet_collides_player(Bullet bull);
	void to_bin();
	int from_bin(char* bobj);
    bool game_over();
    // Variables
    static const size_t SIZE = sizeof(int16_t) * 9;
    int16_t x1 = 250;
    int16_t y1 = 50;
    int16_t x2 = 250;
    int16_t y2 = 450;
    int16_t upperLimit = 5;
    int16_t lowerLimit = 495;
    int16_t playerRadius = 20;
    int16_t bulletRadius = 5;
    int16_t player1Lives = 5;
    int16_t player2Lives = 5;
    std::vector<Bullet> bullets;
};