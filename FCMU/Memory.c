#include <stdlib.h>

#include "Types.h"
#include "Memory.h"


void init_map() {
	time_t t;
	srand((unsigned)time(&t));
	map = malloc(sizeof(byte) * 0xFFFF); //allocate a 16bit address space
	for (int i = 0; i < 0xFFFF; i++) map[i] = rand() % 256;
	//return;

	//hardcoded tests
	//intialize background map 0
	for (int i = 0; i < 1024; i++) {
		map[0xB600 + i] = 0;
	}
	map[0xb600] = 0;

	map[0xb600 + 32] = 0;
	//intiialize default colors
	for (int i = 0; i < 256; i++) {
		map[0xb500 + i] = 0;
	}
	//set up the grid tile 
	byte solid = 0xFF;
	byte left = 0xc0;
	byte right = 0x0;
	byte gridTile[16];
	for (int i = 0; i < 8; i++) {
		gridTile[i*2] = left;
		gridTile[i*2 + 1] = right;
	}
	gridTile[0] = solid;
	gridTile[1] = solid;

	for (int i = 0; i < 16; i++) {
		map[0xa500 + i] = gridTile[i];
	}
	
	byte white[] = { 255, 255, 255 };
	byte black[] = { 0,0,0 };
	byte green[] = { 0, 255, 0 };
	byte red[] = { 255, 0, 0 };
	byte blue[] = { 0, 0, 255 };

	//initialize palette with colors
	//for each of the 64 palettes
	for (int i = 0; i < 64; i++) {
		int paletteStart = i * 12; //(one palette is 12 bytes)
		word addr = 0xd400;
		addr += paletteStart;
		//color 00
		for (int j = 0; j < 3; j++) {
			map[addr++] = blue[j];
		}
		//color 01
		for (int j = 0; j < 3; j++) {
			map[addr++] = 0;
		}
		//color 10
		for (int j = 0; j < 3; j++) {
			map[addr++] = 0;
		}
		//color 11
		for (int j = 0; j < 3; j++) {
			map[addr++] = green[i];
		}
	}
}

