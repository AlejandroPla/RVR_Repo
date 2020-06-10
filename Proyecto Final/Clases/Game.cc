#include <cmath>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include "Game.h"
void Game::movePlayer(bool player, int16_t x, int16_t y) {
    if (!player) {  // Jugador 1
        if (player1->pos_x + x + playerRadius <= 500 && player1->pos_x + x - playerRadius >= 0) {
            player1->pos_x += x;
        }
        if (player1->pos_y + y - playerRadius >= upperLimit) {
            player1->pos_y += y;
        }
        if (player1->pos_y + playerRadius >= 250) {
            player1->lives--;
            player1->pos_x = 250;
            player1->pos_y = 50;
        }
    } else { // Jugador 2
        if (player2->pos_x + x + playerRadius <= 500 && player2->pos_x + x - playerRadius >= 0) {
            player2->pos_x += x;
        }
        if (player2->pos_y + y - playerRadius <= lowerLimit) {
            player2->pos_y += y;
        }
        if (player2->pos_y + playerRadius <= 250) {
            player2->lives--;
            player2->pos_x = 250;
            player2->pos_y = 450;
        }
    }
}
void Game::playerHit(bool player) {
    if (!player) {
        upperLimit += 50;
        // El limite empuja al jugador
        if (player1->pos_y - playerRadius < upperLimit) {
            player1->pos_y = upperLimit + playerRadius;
            if (player1->pos_y + playerRadius >= 250) {
                player1->lives--;
                player1->pos_x = 250;
                player1->pos_y = 50;
                upperLimit = 5;
            }
        }
    } else {
        lowerLimit -= 50;
        // El limite empuja al jugador
        if (player2->pos_y + playerRadius > lowerLimit) {
            player2->pos_y = lowerLimit - playerRadius;
            if (player2->pos_y - playerRadius <= 250) {
                player2->lives--;
                player2->pos_x = 250;
                player2->pos_y = 450;
                lowerLimit = 5;
            }
        }
    }
}
int Game::bullet_collides_player(Bullet bull) {
    // Choca con el jugador 1
    int16_t dist = std::sqrt(
        std::abs(bull.bullet_x - player1->pos_x) * std::abs(bull.bullet_x - player1->pos_x) +
        std::abs(bull.bullet_y - player1->pos_y) * std::abs(bull.bullet_y - player1->pos_y)
    );
    if (dist < playerRadius + bulletRadius) { return 0; }
    // Choca con el jugador 2
    dist = std::sqrt(
        std::abs(bull.bullet_x - player2->pos_x) * std::abs(bull.bullet_x - player2->pos_x) +
        std::abs(bull.bullet_y - player2->pos_y) * std::abs(bull.bullet_y - player2->pos_y)
    );
    if (dist < playerRadius + bulletRadius) { return 1; }
    // No choca
    return -1;
}
void Game::to_bin() {
    // Reserva el tamaño de 9 enteros + el del array de bullets
    size_t bulletsSize = bullets.size() * sizeof(int16_t) * 2;
    alloc_data(SIZE + bulletsSize);
    char* dt = _data;
    memcpy(dt, &player1->pos_x, sizeof(int16_t));          
    dt += sizeof(int16_t);
    memcpy(dt, &player1->pos_y, sizeof(int16_t));          
    dt += sizeof(int16_t); 
    memcpy(dt, &player2->pos_x, sizeof(int16_t)); 
    dt += sizeof(int16_t);
    memcpy(dt, &player2->pos_y, sizeof(int16_t));          
    dt += sizeof(int16_t);
    memcpy(dt, &upperLimit, sizeof(int16_t)); 
    dt += sizeof(int16_t); 
    memcpy(dt, &lowerLimit, sizeof(int16_t));
    dt += sizeof(int16_t);
    memcpy(dt, &player1->lives, sizeof(int16_t));          
    dt += sizeof(int16_t);
    memcpy(dt, &player2->lives, sizeof(int16_t));          
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
    int16_t x1, y1, x2, y2;
    memcpy(&x1, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&y1, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&x2, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&y2, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    player1 = new Player(x1, y1);
    player2 = new Player(x2, y2);
    memcpy(&upperLimit, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&lowerLimit, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&player1->lives, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&player2->lives, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    int16_t s;
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
bool Game::game_over() {
    if (player1->lives <= 0) {
        winning_player = "JUGADOR 2";
        return true;
    }
    else if (player1->lives <= 0) {
        winning_player = "JUGADOR 1";
        return true;
    }

    return false;
}
void Game::reset() {
    bullets.clear();
    player1->lives = 3;
    player2->lives = 3;
    player1->pos_x = 250;
    player1->pos_y = 50;
    player2->pos_x = 250;
    player2->pos_y = 450;
    upperLimit = 5;
    lowerLimit = 495;
}