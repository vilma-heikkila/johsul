/*
 * Pixel.c
 *
 *  Created on: -----
 *      Author: -----
 */

typedef _Bool bool; // TODO: where's my <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include "Pixel.h"

// Table for pixel dots
//                    height  width  RGB
volatile uint8_t dots[DOTS_X][DOTS_Y][3] = {0};

// Helper function for bit manipulation
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
	set_bit(CTRL, CRTL_RST, true);

	// SDA controls the value being sent via serial line.
	set_bit(CTRL, CTRL_SDA, true);

	// For each 24-bit channel in DM163, set the 6-bit register to 0b11111
	for (size_t i = 0; i < DM163_CHANNELS * DM163_BANK0_BITS; i++) {
		// DM163 reads SDA to shift register on rising edge of CSK
		set_bit(CTRL, CTRL_CSK, false);
		set_bit(CTRL, CTRL_CSK, true); // read
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
		for (size_t rgb_channel = 0; rgb_channel < 3; rgb++) {
			uint8_t colour_data = dots[x][y][rgb_channel];
			// Read colour to shift register
			for (uint8_t i; i < DM163_BANK_BITS; i++) {
				set_bit(CTRL, CTRL_SDA, colour_data & bit(8) != 0);
				// Write SDA to shift register on rising edge of CLK
				set_bit(CTRL, CTRL_CLK, false);
				set_bit(CTRL, CTRL_CLK, true);
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

