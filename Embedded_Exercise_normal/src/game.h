#include "platform.h"
#include "xgpiops.h"
#include "xttcps.h"
#include "xscugic.h"
#include "xparameters.h"

typedef _Bool bool;

#define MAX_SCORE 2

#define SHIP_ROW 7
#define ALIEN_ROW 0
#define SCORE_COL 3

#define BTN_LEFT    bit(3)
#define BTN_RIGHT   bit(2)
#define BTN_SHOOT   bit(1)
#define BTN_RESTART bit(0)

enum Direction { LEFT = -1, RIGHT = 1 };

struct Game {
	struct Alien* alien;
	struct Ship* ship;
	struct Projectile* projectile;
	uint8_t score;
	bool end;
};

struct Alien {
	size_t location;
	enum Direction direction;
};

struct Ship {
	size_t location;
	bool shoot_active;
};

struct Projectile {
	size_t x;
	size_t y;
};

// Game functionality
void set_alien_pixels();
void reset_alien_pixels();
void check_alien_direction();
void move_alien();

void set_ship_pixels();
void reset_ship_pixels();
bool ship_move_ok(struct Ship* ship, enum Direction direction);
void move_ship(struct Ship* ship, enum Direction direction);

void check_hit(struct Game* game);
void set_projectile_pixels(struct Projectile* projectile);
void reset_projectile_pixels(struct Projectile* projectile);
void move_projectile(struct Game* game);

void set_score_pixels(uint8_t score);
void reset_all_pixels();

void set_end_pixels();

void restart_game();

