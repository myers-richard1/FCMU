#include <stdlib.h>

#include "Types.h"
#include "Memory.h"


void init_map() {
	map = malloc(sizeof(byte) * 0xFFFF); //allocate a 16bit address space
	for (int i = 0; i < 0xFFFF; i++) map[i] = 0xFF;

	//hardcoded tests
	for (int i = 0; i < 1024; i++) {
		map[0xB600 + i] = 0;
	}
	for (int i = 0; i < 256; i++) {
		map[0xb500 + i] = 0;
	}
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
}

