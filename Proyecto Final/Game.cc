#include <cmath>
#include <cstring>
#include "Game.h"

void Game::movePlayer(bool player, int16_t x, int16_t y) {
    // Player 1
    if (!player) {
        if (player1->x + x + playerRadius <= 500 && player1->x + x - playerRadius >= 0) {
            player1->x += x;
        }
        if (player1->y + y - playerRadius >= upperLimit) {
            player1->y += y;
        }
        if (player1->y + playerRadius >= 250) {
            player1->lives--;
            player1->x = 250;
            player1->y = 50;
            upperLimit = 5;
        }
    // Player 2
    } else {
        if (player2->x + x + playerRadius <= 500 && player2->x + x - playerRadius >= 0) {
            player2->x += x;
        }
        if (player2->y + y + playerRadius <= lowerLimit) {
            player2->y += y;
        }
        if (player2->y - playerRadius <= 250) {
            player2->lives--;
            player2->x = 250;
            player2->y = 450;
            lowerLimit = 495;
        }
    }
}

void Game::playerHit(bool player) {
    // Player 1
    if (!player) {
        upperLimit += 50;

        // Limit pushes player
        if (player1->y - playerRadius < upperLimit) {
            player1->y = upperLimit + playerRadius;
            if (player1->y + playerRadius >= 250) {
                player1->lives--;
                player1->x = 250;
                player1->y = 50;
                upperLimit = 5;
            }
        }
    // Player 2
    } else {
        lowerLimit -= 50;

        // Limit pushes player
        if (player2->y + playerRadius > lowerLimit) {
            player2->y = lowerLimit - playerRadius;
            if (player2->y - playerRadius <= 250) {
                player2->lives--;
                player2->x = 250;
                player2->y = 450;
                lowerLimit = 495;
            }
        }
    }
}

int Game::bullet_collides_player(Bullet bull) {
    // Collides with Player 1
    int16_t dist = std::sqrt(
        std::abs(bull.x - player1->x) * std::abs(bull.x - player1->x) +
        std::abs(bull.y - player1->y) * std::abs(bull.y - player1->y)
    );
    if (dist < playerRadius + bulletRadius) { return 0; }

    // Collides with Player 2
    dist = std::sqrt(
        std::abs(bull.x - player2->x) * std::abs(bull.x - player2->x) +
        std::abs(bull.y - player2->y) * std::abs(bull.y - player2->y)
    );
    if (dist < playerRadius + bulletRadius) { return 1; }

    // Doesn't collide
    return -1;
}

void Game::to_bin() {
    size_t bulletsSize = bullets.size() * sizeof(int16_t) * 2;
    alloc_data(SIZE + bulletsSize);
    char* dt = _data;

    // Players' positions
    memcpy(dt, &player1->x, sizeof(int16_t));          
    dt += sizeof(int16_t);
    memcpy(dt, &player1->y, sizeof(int16_t));          
    dt += sizeof(int16_t); 
    memcpy(dt, &player2->x, sizeof(int16_t)); 
    dt += sizeof(int16_t);
    memcpy(dt, &player2->y, sizeof(int16_t));          
    dt += sizeof(int16_t);

    // Game area limits
    memcpy(dt, &upperLimit, sizeof(int16_t)); 
    dt += sizeof(int16_t); 
    memcpy(dt, &lowerLimit, sizeof(int16_t));
    dt += sizeof(int16_t);

    // Players' lives
    memcpy(dt, &player1->lives, sizeof(int16_t));          
    dt += sizeof(int16_t);
    memcpy(dt, &player2->lives, sizeof(int16_t));          
    dt += sizeof(int16_t);

    // Bullets array size
    int16_t s = bullets.size();
    memcpy(dt, &s, sizeof(int16_t));

    // Bullets' positions
    for (int i = 0; i < bullets.size(); i++) {
        dt += sizeof(int16_t); 
        memcpy(dt, &bullets[i].x, sizeof(int16_t));
        dt += sizeof(int16_t); 
        memcpy(dt, &bullets[i].y, sizeof(int16_t));
    }
}

int Game::from_bin(char * bobj) {
    // Players' positions
    int16_t x1, y1, x2, y2;
    memcpy(&x1, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&y1, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&x2, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&y2, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    player1 = new Player(0, x1, y1, playerRadius);
    player2 = new Player(1, x2, y2, playerRadius);

    // Game area limits
    memcpy(&upperLimit, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&lowerLimit, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);

    // Players' lives
    memcpy(&player1->lives, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&player2->lives, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    
    // Bullets array size
    int16_t s;
    memcpy(&s, bobj, sizeof(int16_t));
    bullets.clear();

    // Bullets' positions
    for (int i = 0; i < s; i++) {
        int16_t x;
        int16_t y;
        bobj += sizeof(int16_t); 
        memcpy(&x, bobj, sizeof(int16_t));
        bobj += sizeof(int16_t); 
        memcpy(&y, bobj, sizeof(int16_t));
        Bullet bull(x, y, 0, bulletRadius, bulletSpeed);
        bullets.push_back(bull);
    }

    return 0;
};

bool Game::game_over() {
    // Player 1 lost
    if (player1->lives == 0) {
        winning_player = "JUGADOR 2";
        return true;
    }
    // Player 2 lost
    else if (player2->lives == 0) {
        winning_player = "JUGADOR 1";
        return true;
    }

    return false;
}

void Game::reset() {
    bullets.clear();
    player1->lives = 3;
    player2->lives = 3;
    player1->x = 250;
    player1->y = 50;
    player2->x = 250;
    player2->y = 450;
    upperLimit = 5;
    lowerLimit = 495;
}