/*
 * Pixel.c
 *
 *  Created on: -----
 *      Author: -----
 */

#include "Pixel.h"

#define ctrl *(uint8_t*) 0x41220008
#define channel *(uint8_t*) 0x41220000


//Table for pixel dots.
//				 dots[X][Y][COLOR]
volatile uint8_t dots[8][8][3]={0};


// Here the setup operations for the LED matrix will be performed
void setup(){
	ctrl = 0;
	channel = 0;

	//reseting screen at start is a MUST to operation (Set RST-pin to 1).
	ctrl |= 0x01;	// RST is bit0  -> 0b00001
	usleep(500);

	ctrl &=~ 0x01;
	usleep(500);

	ctrl |= 0x01;

	ctrl |= 0x10;  // SDA is bit4 -> 0b10001

	//Write code that sets 6-bit values in register of DM163 chip. Recommended that every bit in that register is set to 1. 6-bits and 24 "bytes", so some kind of loop structure could be nice.
	//24*6 bits needs to be transmitted

	for(uint8_t data_counter = 0; data_counter < 144; data_counter++) {

		ctrl &=~ 0x08; // bit3 to 0
		ctrl |= 0x08; // bit3 to 1
	}

	//Final thing in this function is to set SB-bit to 1 to enable transmission to 8-bit register.
	ctrl |= 0x04;	// SB is bit2 -> 0b10101
}

//Change value of one pixel at led matrix. This function is only used for changing values of dots array
void SetPixel(uint8_t x,uint8_t y, uint8_t r, uint8_t g, uint8_t b){

	//Hint: you can invert Y-axis quite easily with 7-y
	dots[x][y][0]=b;
	//Write rest of two lines of code required to make this function work properly (green and red colors to array).
	dots[x][y][1]=g;
	dots[x][y][2]=r;
}


//Put new data to led matrix. Hint: This function is supposed to send 24-bytes and parameter x is for channel x-coordinate.
void run(uint8_t x){
	ctrl &=~ 0x02; // bit1 to 0

	//Write code that writes data to led matrix driver (8-bit data). Use values from dots array
	//Hint: use nested loops (loops inside loops)
	//Hint2: loop iterations are 8,3,8 (pixels,color,8-bitdata)
	for(uint8_t i = 0; i < 8; i++) {

		for(uint8_t k = 0; k < 3; k++) {
			uint8_t colour_data = dots[x][i][k];

			for(uint8_t j; j < 8; j++) {

				if (colour_data & 0x80) ctrl|=0x10; // if bit8 in dots is set to 1...
				else ctrl&=~0x10;

				ctrl&=~0x08; // CLK to 0
				ctrl|=0x08;  // CLK to 1
				colour_data<<=1;
			}
		}
	}

	latch();
}

//Latch signal. See colorsshield.pdf or DM163.pdf in project folder on how latching works
void latch(){
	ctrl |= 0x02; 	// Lat bit is bit1 -> 0b00010
	ctrl &=~ 0x02;

}


//Set one line (channel) as active, one at a time.
void open_line(uint8_t x){
	switch(x) {
		case 0: channel|=0x01; // 0b00000001
		case 1: channel|=0x02; // 0b00000010
		case 2: channel|=0x04; // 0b00000100
		case 3: channel|=0x08; // 0b00001000
		case 4: channel|=0x10; // 0b00010000
		case 5: channel|=0x20; // 0b00100000
		case 6: channel|=0x40; // 0b01000000
		case 7: channel|=0x80; // 0b10000000
		default: channel = 0;
	}

}


