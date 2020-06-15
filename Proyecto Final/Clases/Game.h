#include <vector>
#include "GameObjects.h"
#include "Serializable.h"

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
    std::vector<GameObject*> game_objects;
    std::string winning_player;

    // Metodos
    Game() {
        player1 = new Player(0, 250, 50, playerRadius);
        player2 = new Player(1, 250, 450, playerRadius);
        game_objects.push_back(player1);
        game_objects.push_back(player2);
    };

	void movePlayer(bool player, int16_t x, int16_t y);
	void playerHit(bool player);
	int bullet_collides_player(Bullet bull);
	void to_bin();
	int from_bin(char* bobj);
    bool game_over();
    void reset();
};