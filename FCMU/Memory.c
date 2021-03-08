#include <stdlib.h>

#include "Types.h"
#include "Memory.h"


void init_map() {
	map = malloc(sizeof(byte) * 0xFFFF); //allocate a 16bit address space
	for (int i = 0; i < 0xFFFF; i++) map[i] = 0xFF;
}