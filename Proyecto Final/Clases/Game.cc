#include <cmath>
#include <string.h>
#include <string>
#include <unistd.h>
#include "Game.h"
void Game::movePlayer(bool player, int16_t x, int16_t y) {
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
void Game::playerHit(bool player) {
	if (!player) {
		upperLimit += 50;
		// El limite empuja al jugador
		if (y1 - playerRadius < upperLimit) {
			y1 = upperLimit + playerRadius;
			// El jugador pasa la linea del centro
			if (y1 + playerRadius >= 250) {
				printf("PIERDE EL JUGADOR 1\n");
			}
		}
	} else {
		lowerLimit -= 50;
		// El limite empuja al jugador
		if (y2 + playerRadius > lowerLimit) {
			y2 = lowerLimit - playerRadius;
			// El jugador pasa la linea del centro
			if (y2 - playerRadius <= 250) {
				printf("PIERDE EL JUGADOR 2\n");
			}
		}
	}
}

int Game::bullet_collides_player(Bullet bull) {
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

void Game::to_bin() {
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
int Game::from_bin(char * bobj) {
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