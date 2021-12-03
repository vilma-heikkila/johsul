/*
 * Pixel.h
 *
 *  Created on: ------
 *      Author: ------
 */

#ifndef PIXEL_H_
#define PIXEL_H_

#include "platform.h"
#include "xil_printf.h"
#include "sleep.h"
#include "xgpiops.h"
#include "xttcps.h"
#include "xscugic.h"
#include "xparameters.h"

typedef _Bool bool;
enum { false = 0, true = 1 };

/*
 * Macro definitions
 */

// LED controller specs
#define DM163_CHANNELS 24
#define DM163_BANK0_BITS 6
#define DM163_BANK1_BITS 8

// LED matrix specs
#define DOTS_X 8
#define DOTS_Y 8
// Color channel order
#define B 0
#define G 1
#define R 2

/* Important memory addresses and bits */
#define bit(n) (0x01 << n)

// LED matrix channels
#define CHANNEL ((uint8_t*) 0x41220008)
#define NO_LINE 8

// Control signals
#define CTRL ((uint8_t*) 0x41220008)
#define CTRL_RST bit(0)
#define CTRL_LAT bit(1)
#define CTRL_SB  bit(2)
#define CTRL_SCK bit(3)
#define CTRL_SDA bit(4)

// Onboard buttons and switches
#define INPUT ((uint8_t*) 0xe000a068)
#define INPUT_BTN0 bit(0)
#define INPUT_BTN1 bit(1)
#define INPUT_BTN2 bit(2)§
#define INPUT_BTN3 bit(3)
#define INPUT_SW0  bit(4)
#define INPUT_SW1  bit(5)

// Onboard monochrome LEDs
#define BOARD_LED ((uint8_t*) 0x41200000)
#define BOARD_LED0 bit(0)
#define BOARD_LED1 bit(1)
#define BOARD_LED2 bit(2)
#define BOARD_LED3 bit(3)

// Onboard RGB LEDs
#define BOARD_LED_RGB
#define BOARD_LED4_B bit(0)
#define BOARD_LED4_G bit(1)
#define BOARD_LED4_R bit(2)
#define BOARD_LED5_B bit(3)
#define BOARD_LED5_G bit(4)
#define BOARD_LED5_R bit(5)

void setup();
void SetPixel(uint8_t x,uint8_t y, uint8_t r, uint8_t g, uint8_t b);
void run(uint8_t x);
void latch();
void open_line(uint8_t x);

#endif /* PIXEL_H_ */
