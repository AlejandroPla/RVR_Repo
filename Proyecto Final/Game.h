#include <vector>
#include "GameObjects.h"
#include "Serializable.h"

class Game : public Serializable {
public:
    static const size_t SIZE = sizeof(int16_t) * 9;
    
    Player* player1;
    Player* player2;

    int16_t playerRadius = 20;
    int16_t bulletRadius = 5;
    int16_t bulletSpeed = 1;

    int16_t upperLimit = 5;
    int16_t lowerLimit = 495;

    std::vector<Bullet> bullets;
    std::string winning_player;

    Game() {
        player1 = new Player(0, 250, 50, playerRadius);
        player2 = new Player(1, 250, 450, playerRadius);
    };

	void to_bin();
	int from_bin(char* bobj);

	void movePlayer(bool player, int16_t x, int16_t y);
	void playerHit(bool player);
	int bullet_collides_player(Bullet bull);
    bool game_over();
    void reset();
};