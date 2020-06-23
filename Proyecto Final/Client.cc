#include "Client.h"
#include "Message.h"
#include <unistd.h>

void Client::login() {
    std::string msg;
    Message em(nick);
    em.type = Message::LOGIN;
    socket.send(em, socket);
}

void Client::logout() {
	std::string msg;
    Message em(nick);
    em.type = Message::LOGOUT;
    socket.send(em, socket);
}

void Client::input_thread() {
	char k;
	Message em(nick);
	while (!exit) {
        k = dpy->wait_key();
        switch(k) {
			case 'w':
				if (!game->game_over()) {
					em.type = Message::UP;
					socket.send(em, socket);
				}
			break;
			case 'a':
				if (!game->game_over()) {
					em.type = Message::LEFT;
					socket.send(em, socket);
				}
			break;
			case 's':
				if (!game->game_over()) {
					em.type = Message::DOWN;
					socket.send(em, socket);
				}
			break;
            case 'd':
				if (!game->game_over()) {
					em.type = Message::RIGHT;
					socket.send(em, socket);
				}
			break;
			case ' ':
				if (!game->game_over()) {
					em.type = Message::SHOOT;
					socket.send(em, socket);
				}
			break;
            case 'q':
				exit = true;
				logout();
			break;
			case 'r':
				if (game->game_over()) {
					em.type = Message::RESET;
					socket.send(em, socket);
				}
			break;
		}
    }
}

void Client::net_thread() {
    while(!exit) {
		socket.recv(*game);
    }
}

void Client::render_thread() {
    while(!exit) {
        usleep(10000);
		dpy->clear();

		if (!game->game_over()) {
			// Players
			game->player1->render();
			game->player2->render();

			// Lives
            dpy->set_color(XLDisplay::RED);
			std::string s1 = "Player 1: " + std::to_string(game->player1->lives);
			std::string s2 = "Player 2: " + std::to_string(game->player2->lives);
			dpy->text(50, 20, s1);
			dpy->text(350, 480, s2);

			// Middle line
			dpy->set_color(XLDisplay::RED);
			dpy->line(0, 250, 500, 250);

			// Upper and lower limits
            dpy->set_color(XLDisplay::BLACK);
			dpy->line(0, game->upperLimit, 500, game->upperLimit);
			dpy->line(0, game->lowerLimit, 500, game->lowerLimit);
			
            // Bullets
			for (int i = 0; i < game->bullets.size(); i++) {
				game->bullets[i].render();
			}
		} else {
			dpy->set_color(XLDisplay::RED);
			std::string winner = "Gana " + game->winning_player;
			dpy->text(200, 250, winner);
			dpy->text(200, 260, "Pulsa R para reiniciar");
		}
        dpy->flush();
    }
}