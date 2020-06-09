#include "Client.h"
void Client::login() {
    std::string msg;
    ChatMessage em(nick);
    em.type = ChatMessage::LOGIN;
    socket.send(em, socket);
}
void Client::logout() {
	std::string msg;
    ChatMessage em(nick);
    em.type = ChatMessage::LOGOUT;
    socket.send(em, socket);
}
void Client::input_thread() {
	char k;
	ChatMessage em(nick);
	do {
        k = dpy->wait_key();
        switch(k) {
			case 'w':
				em.type = ChatMessage::UP;
				socket.send(em, socket);
			break;
			case 'a':
				em.type = ChatMessage::LEFT;
				socket.send(em, socket);
			break;
			case 's':
				em.type = ChatMessage::DOWN;
				socket.send(em, socket);
			break;
			case 'd':
				em.type = ChatMessage::RIGHT;
				socket.send(em, socket);
			break;
			case ' ':
				em.type = ChatMessage::SHOOT;
				socket.send(em, socket);
			break;
			case 'q':
				logout();
				exit = true;
			break;
		}
    } while (!game->game_over() || !exit);
}
void Client::net_thread() {
    while(!game->game_over()) {
		socket.recv(*game);
		dpy->clear();
		// Players
		dpy->set_color(XLDisplay::BLUE);
		dpy->circle(game->x1, game->y1, game->playerRadius);
		dpy->set_color(XLDisplay::GREEN);
		dpy->circle(game->x2, game->y2, game->playerRadius);
		// Lives
	    dpy->set_color(XLDisplay::RED);
		std::string s1 = "Player 1: " + std::to_string(game->player1Lives);
		std::string s2 = "Player 2: " + std::to_string(game->player2Lives);
	    dpy->text(50, 20, s1);
		dpy->text(350, 480, s2);
		// Middle line
		dpy->set_color(XLDisplay::RED);
		dpy->line(0, 250, 500, 250);
		// Upper and lower limits
		dpy->set_color(XLDisplay::BLACK);
		dpy->line(0, game->upperLimit, 500, game->upperLimit);
		dpy->line(0, game->lowerLimit, 500, game->lowerLimit);
		for (int i = 0; i < game->bullets.size(); i++) {
			dpy->circle(game->bullets[i].bullet_x, game->bullets[i].bullet_y, game->bulletRadius);
		}
		if (game->game_over()) {
			dpy->clear();
			dpy->set_color(XLDisplay::RED);
			std::string winner = "Gana " + game->winning_player;
			dpy->text(250, 250, winner);
			dpy->flush();
			sleep(1);
		}
    }
}