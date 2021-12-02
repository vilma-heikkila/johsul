/*
 * Pixel.c
 *
 *  Created on: -----
 *      Author: -----
 */

typedef _Bool bool; // TODO: where's my <stdbool.h>

#include "Pixel.h"

// Table for pixel dots
//                    height  width  RGB
volatile uint8_t dots[DOTS_X][DOTS_Y][3] = {0};

// Helper function for bit manipulation
void set_bit(uint8_t*, uint8_t, bool);
inline void set_bit(uint8_t* addr, uint8_t bit, bool value) {
	if (value) {
		*addr |= bit;
	} else {
		*addr &= ~bit;
	}
}

// Here the setup operations for the LED matrix will be performed
void setup() {
	*CTRL = 0x00;
	*CHANNEL = 0x00;

	// Reset the LED matric by turning RST bit on and off
	set_bit(CTRL, CTRL_RST, true);
	usleep(500);
	set_bit(CTRL, CTRL_RST, false);
	usleep(500);
	set_bit(CTRL, CTRL_RST, true);

	// SDA controls the value being sent via serial line.
	set_bit(CTRL, CTRL_SDA, true);

	// For each 24-bit channel in DM163, set the 6-bit register to 0b11111
	for (size_t i = 0; i < DM163_CHANNELS * DM163_BANK0_BITS; i++) {
		// DM163 reads SDA to shift register on rising edge of CSK
		set_bit(CTRL, CTRL_SCK, false);
		set_bit(CTRL, CTRL_SCK, true); // read
	}

	// Set SB to 1 to enable transmission to 8-bit register.
	set_bit(CTRL, CTRL_SB, true);
}

// Change value of one pixel at led matrix.
// This function is only used for changing values of dots array
void SetPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
	dots[x][y][B]=b;
	dots[x][y][G]=g;
	dots[x][y][R]=r;
}

// Latch signal
// Both shift registers are latched on falling edge of LAT
void latch() {
	set_bit(CTRL, CTRL_LAT, true);
	set_bit(CTRL, CTRL_LAT, false); // latch
}

// Put new data to led matrix. Hint: This function is supposed to send 24-bytes
// and parameter x is for CHANNEL x-coordinate.
void run(uint8_t x) {
	set_bit(CTRL, CTRL_LAT, false);

	// Write code that writes data to led matrix driver (8-bit data). Use values from dots array
	// Write colours from dots arrau to DM163
	for (size_t y = 0; y < DOTS_Y; y++) {
		for (size_t rgb_channel = 0; rgb_channel < 3; rgb_channel++) {
			uint8_t colour_data = dots[x][y][rgb_channel];
			// Read colour to shift register
			for (uint8_t i; i < DM163_BANK1_BITS; i++) {
				set_bit(CTRL, CTRL_SDA, colour_data & bit(8) != 0);
				// Write SDA to shift register on rising edge of CLK
				set_bit(CTRL, CTRL_SCK, false);
				set_bit(CTRL, CTRL_SCK, true);
				colour_data <<= 1;
			}
		}
	}
	latch();
}

// Set one line (CHANNEL) as active, one at a time.
void open_line(uint8_t x){
	// If x>7, bit(x) overflows and is set to 0
	*CHANNEL = bit(x);
}

void set_alien_pixels() {
	SetPixel(ALIEN_LOC, 0, 255, 0, 0);
}

void reset_alien_pixels() {
	SetPixel(ALIEN_LOC, 0, 0, 0, 0);
}

void check_alien_direction() {
	if (ALIEN_LOC + 1 > 7) {
		ALIEN_DIR = LEFT;
	}

	if (ALIEN_LOC - 1 < 0) {
		ALIEN_DIR = RIGHT;
	}
}

void move_alien() {
	check_alien_direction();

	ALIEN_LOC += ALIEN_DIR;

	set_alien_pixels();
}

void set_ship_pixels() {
	//      O
	//    O O O

	SetPixel(SHIP_LOC, 7, 0, 0, 255);
	SetPixel(SHIP_LOC-1, 7, 0, 0, 255);
	SetPixel(SHIP_LOC+1, 7, 0, 0, 255);
	SetPixel(SHIP_LOC, 6, 0, 0, 255);
}

void reset_ship_pixels() {
	SetPixel(SHIP_LOC, 7, 0, 0, 0);
	SetPixel(SHIP_LOC-1, 7, 0, 0, 0);
	SetPixel(SHIP_LOC+1, 7, 0, 0, 0);
	SetPixel(SHIP_LOC, 6, 0, 0, 0);
}

bool ship_move_ok(uint8_t direction) {
	if (direction == LEFT) {
		if (SHIP_LOC - 1 < 0) {
			return false;
		}
		else {
			return true;
		}
	}

	else {
		if (SHIP_LOC + 1 > 7) {
			return false;
		}
		else {
			return true;
		}
	}
}

void move_ship(uint8_t direction) {
	SHIP_LOC += direction;
	set_ship_pixels();
}

void check_hit() {
	if (ALIEN_LOC == PROJECTILE_X) {
		SCORE++;
		set_score_pixels(SCORE);
	}
	if (SCORE == MAX_SCORE) {
		GAME_END = true;
	}

}

void set_projectile_pixels() {
	SetPixel(PROJECTILE_X, PROJECTILE_Y, 0, 255, 0);
}

void reset_projectile_pixels() {
	SetPixel(PROJECTILE_X, PROJECTILE_Y, 0, 0, 0);
}

void move_projectile() {
	PROJECTILE_Y--;
	if (PROJECTILE_Y == 0) {
		check_hit();
		PROJECTILE_Y = 5;
		SHOOT_ACTIVE = false;
	}

	else {
		set_projectile_pixels();
	}
}

void set_score_pixels(uint8_t score) {
	SetPixel(7, score, 0, 255, 0);
}

void reset_all_pixels() {
	for (uint8_t x = 0; x < 7; x++) {
		for (uint8_t y = 0; y < 7; y++) {
			SetPixel(x, y, 0, 0, 0);
		}
	}
}

void set_end_pixels() {
	SetPixel(2, 2, 0, 255, 0);
	SetPixel(3, 3, 0, 255, 0);
	SetPixel(5, 2, 0, 255, 0);
	SetPixel(4, 3, 0, 255, 0);
	SetPixel(3, 4, 0, 255, 0);
	SetPixel(4, 4, 0, 255, 0);
	SetPixel(2, 5, 0, 255, 0);
	SetPixel(5, 5, 0, 255, 0);
}

void restart_game() {
	reset_all_pixels();

	// Set initial values
	ALIEN_LOC = 3;
	ALIEN_DIR = 1;
	SHIP_LOC = 3;
	PROJECTILE_X = 0;
	PROJECTILE_Y = 5;
	SCORE = 0;
	SHOOT_ACTIVE = false;
	GAME_END = false;

	set_alien_pixels();
	set_ship_pixels();
}
