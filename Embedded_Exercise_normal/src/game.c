#include "game.h"
#include "pixel.h"

void set_alien_pixels(struct Alien* alien) {
	set_pixel(alien->location, ALIEN_ROW, 0xff, 0x00, 0x00);
}

void reset_alien_pixels(struct Alien* alien) {
	set_pixel(alien->location, ALIEN_ROW, 0x00, 0x0, 0x00);
}

void check_alien_direction(struct Alien* alien) {
	if (alien->location >= DOTS_X) {
		alien->direction = LEFT;
	}
	if (alien->location <= 0) {
		alien->direction = RIGHT;
	}
}

void move_alien(struct Alien* alien) {
	check_alien_direction(alien);
	alien->location += (size_t)alien->direction;
	set_alien_pixels(alien);
}

void set_ship_pixels(struct Ship* ship) {
	//   O 
	// O O O
	set_pixel(ship->location,     SHIP_ROW,     0x00, 0x00, 0xff);
	set_pixel(ship->location - 1, SHIP_ROW,     0x00, 0x00, 0xff);
	set_pixel(ship->location + 1, SHIP_ROW,     0x00, 0x00, 0xff);
	set_pixel(ship->location,     SHIP_ROW - 1, 0x00, 0x00, 0xff);
}

void reset_ship_pixels(struct Ship* ship) {
	set_pixel(ship->location,     SHIP_ROW    , 0x00, 0x00, 0x00);
	set_pixel(ship->location - 1, SHIP_ROW    , 0x00, 0x00, 0x00);
	set_pixel(ship->location + 1, SHIP_ROW    , 0x00, 0x00, 0x00);
	set_pixel(ship->location,     SHIP_ROW - 1, 0x00, 0x00, 0x00);
}

bool ship_move_ok(struct Ship* ship, enum Direction direction) {
	switch (direction) {
		case LEFT:
			return (ship->location > 0);
		case RIGHT:
			return (ship->location < DOTS_X);
		default:
			return false; // Unreachable
	}
}

void move_ship(struct Ship* ship, enum Direction direction) {
	ship->location += (size_t)direction;
	set_ship_pixels(ship);
}

void check_hit(struct Game* game) {
	if (game->alien->location == game->projectile->x) {
		game->score++;
		set_score_pixels(game);
	}
	if (game->score == MAX_SCORE) {
		game->end = true;
	}
}

void set_projectile_pixels(struct Projectile* projectile) {
	set_pixel(projectile->x, projectile->y, 0x00, 0xff, 0x00);
}

void reset_projectile_pixels(struct Projectile* projectile) {
	set_pixel(projectile->x, projectile->y, 0x00, 0x00, 0x00);
}

move_projectile(struct Game* game) {
	game->projectile->y--;
	if (game->projectile->y == 0) {
		check_hit(game);
		game->projectile->y = 5;
		game->ship->shoot_active = false;
	} else {
		set_projectile_pixels(game->projectile);
	}
}

void set_score_pixels(uint8_t score) {
	set_pixel(SCORE_COL, score, 0x00, 0xff, 0x00);
}

void reset_all_pixels() {
	for (size_t x = 0; x < DOTS_X; x++) {
		for (size_t y = 0; y < DOTS_Y; y++) {
			set_pixel(x, y, 0x00, 0x00, 0x00);
		}
	}
}

void set_end_pixels() {
	// # 0 1 2 3 4 5 6 7
	// 0
	// 1
	// 2     X     X
	// 3       X X
	// 4       X X
	// 5     X     X
	// 6
	// 7
	set_pixel(2, 2, 0x00, 0xff, 0x00);
	set_pixel(2, 5, 0x00, 0xff, 0x00);
	set_pixel(3, 3, 0x00, 0xff, 0x00);
	set_pixel(3, 4, 0x00, 0xff, 0x00);
	set_pixel(4, 3, 0x00, 0xff, 0x00);
	set_pixel(4, 4, 0x00, 0xff, 0x00);
	set_pixel(5, 2, 0x00, 0xff, 0x00);
	set_pixel(5, 5, 0x00, 0xff, 0x00);
}

void restart_game(struct Game* game) {
	game->alien->location = 3;
	game->alien->direction = RIGHT;
	game->ship->location = 3;
	game->ship->shoot_active = false;
	game->projectile->x = 0;
	game->projectile->y = 5;
	game->score = 0;
	game->end = false;
	set_alien_pixels(game->alien);
	set_ship_pixels(game->ship);
}

