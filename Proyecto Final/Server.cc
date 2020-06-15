#include "Server.h"
#include "Message.h"
#include <unistd.h>

void Server::do_messages() {
    while (true) {
		ChatMessage message;
		Socket* sdMessage;
		socket.recv(message, sdMessage);
		switch(message.type) {
			case ChatMessage::UP:
				if (message.nick == name_player1) {
					game->movePlayer(0, 0, -10);
				} else if (message.nick == name_player2) {
					game->movePlayer(1, 0, -10);
				}			
			break;
			case ChatMessage::DOWN:
				if (message.nick == name_player1) {
					game->movePlayer(0, 0, 10);
				} else if (message.nick == name_player2) {
					game->movePlayer(1, 0, 10);
				}			
			break;
			case ChatMessage::LEFT:
				if (message.nick == name_player1) {
					game->movePlayer(0, -10, 0);
				} else if (message.nick == name_player2) {
					game->movePlayer(1, -10, 0);
				}		
			break;
			case ChatMessage::RIGHT:
				if (message.nick == name_player1) {
					game->movePlayer(0, 10, 0);
				} else if (message.nick == name_player2) {
					game->movePlayer(1, 10, 0);
				}			
			break;
			case ChatMessage::SHOOT:
				int16_t x, y;
				if (message.nick == name_player1) {
					x = game->player1->x;
					y = game->player1->y + game->playerRadius + game->bulletRadius;
					Bullet b1(x, y, 0, game->bulletRadius, game->bulletSpeed);
					game->bullets.push_back(b1);
				} else if (message.nick == name_player2) {
					x = game->player2->x;
					y = game->player2->y - game->playerRadius - game->bulletRadius;
					Bullet b2(x, y, 1, game->bulletRadius, game->bulletSpeed);
					game->bullets.push_back(b2);
				}			
			break;
			case ChatMessage::LOGIN:
				if (client1 == nullptr) {
					client1 = sdMessage;
					name_player1 = message.nick;
					std::cout << "Player 1 (" << *sdMessage << ") logged in\n";
				} else if (client2 == nullptr) {
					client2 = sdMessage;
					name_player2 = message.nick;
					std::cout << "Player 2 (" << *sdMessage << ") logged in\n";
				} else {
					std::cout << *sdMessage << " tried to connect but lobby is full\n";
				}
			break;
			case ChatMessage::LOGOUT:
				if(message.nick == name_player1) {
					client1 = nullptr;
					name_player1 = "";
					std::cout << "Player 1 (" << *sdMessage << ") logged out\n";
				} else if(message.nick == name_player2) {
					client2 = nullptr;
					name_player2 = "";
					std::cout << "Player 2 (" << *sdMessage << ") logged out\n";
				}
			break;
			case ChatMessage::RESET:
				game->reset();
			break;
		}
    }
}

void Server::updateBullets() {
	for (int i = 0; i < game->bullets.size(); i++) {
		game->bullets[i].update();

		// Check border collision
		if (game->bullets[i].check_collision(game->upperLimit, game->lowerLimit)) {
			game->bullets.erase(game->bullets.begin() + i);
		}

		// Check Player1 collision
		if (game->bullet_collides_player(game->bullets[i]) == 0) {
			game->bullets.erase(game->bullets.begin() + i);
			game->playerHit(0);
		}

		// Check Player2 collision
		if (game->bullet_collides_player(game->bullets[i]) == 1) {
			game->bullets.erase(game->bullets.begin() + i);
			game->playerHit(1);
		}
	}
}

void Server::update_thread() {
	while(true) {
		usleep(10000);
		
		// Update bullets
		if (!game->game_over())
			updateBullets();

		// Update Client1
		if (client1 != nullptr)
			socket.send(*game, *client1);

		// Update Client2
		if (client2 != nullptr)
			socket.send(*game, *client2);
	}
}