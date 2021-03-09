#pragma once

#include "cpu.h"
#include "Memory.h"
#include "Types.h"
#include <stdio.h>

//hacky maps for plugging in opcodes and getting functors and operands
functor standardParameterInstructions[] = {ld, ld, ld, ld, ld, ld, ld, ld, add, adc, sub, sbc, cmp, and, or, xor, inc, dec};
byte* standardParameterMap[] = { &rA,  &rB, &rC, &rD, &rE, &rH, &rL, NULL, &rA, &rB, &rC, &rD, &rE, &rH, &rL, NULL};
functor flaggedFunctorMap[] = {jp, call, ret};
Flag flags[] = {none, flagZ, flagC,flagNZ, flagNC};
word* stackOpsMap[] = {&rAF, &rBC, &rDE, &rHL, &rAF, &rBC, &rDE, &rHL};
word* parameterMap16[] = {&rBC, &rDE, &rHL, &sp, &rBC, &rDE, &rHL, &sp, &rBC, &rDE, &rHL, &sp,& rBC,& rDE,& rHL,& sp};
functor wordMathFunctorMap[] = {inc, dec, add};
functor parameterlessMap[] = {ei, di, halt, swap, cpl, bcd};

Instruction currentInstruction;

void init_cpu() {
	sp = 0xFFFF;
	pc = 0;
	rAF = 0;
	rBC = 0;
	rDE = 0; 
	rHL = 0;
}

void test_cpu() {
	rA = 5;
	rB = 2;
	decode(0x01);
	execute();
	printf("A should be 2: 0x%x\n", rA);
	map[0] = 0x40;
	map[1] = 0xF0;
	decode(map[0]);
	execute();
	printf("A should be F0: 0x%x\n", rA);
	getchar();
}

void cycle() {
	if (halted) return;
	//fetch
	byte instruction = map[pc];
	printf("Instruction loaded from pc 0x%x: %x\n", pc, instruction);
	//reset state
	currentInstruction.func = nopfunc;
	currentInstruction.leftByteOperand = NULL;
	currentInstruction.rightByteOperand = NULL;
	currentInstruction.leftWordOperand = NULL;
	currentInstruction.rightWordOperand = NULL;
	currentInstruction.flag = none;
	//decode
	decode(instruction);
	//execute
	execute();
	//increment pc
	pc++;
}

void decode(byte instruction) {
	//standard parameter instructions
	if (instruction < 0x90) {
		currentInstruction.func = standardParameterInstructions[instruction / 8];
		currentInstruction.leftByteOperand = renderPointer(instruction &0x0f);
		currentInstruction.rightByteOperand = renderPointer(instruction & 0x0f);
	}
	//immediate versions of the above instructions
	else if (instruction < 0xA0) {
		currentInstruction.func = standardParameterInstructions[instruction - 0x90];
		currentInstruction.rightByteOperand = &map[++pc];
		if (currentInstruction.func == ld) currentInstruction.leftByteOperand = renderPointer(instruction & 0x0f);
		else currentInstruction.leftByteOperand = &rA;
	}
	//weird 8bit loads
	else if (instruction < 0xB0) {
		printf("Instruction 0x%x found, weird load\n", instruction);
		currentInstruction.func = ld;
		byte* pointer = renderRarePointer(instruction & 0x0f);
		currentInstruction.leftByteOperand = &rA;
		currentInstruction.rightByteOperand = &rA;
		if ((instruction & 0x0f) < 4) currentInstruction.rightByteOperand = pointer;
		else currentInstruction.leftByteOperand = pointer;
	}
	//todo bit
	else if (instruction < 0xB8) {

	}
	//todo rotations and shifts
	else if (instruction < 0xC0) {

	}
	//flow control
	else if (instruction < 0xD8) {
		printf("Flow control instruction\n");
		int reranged = instruction - 0xc0;
		currentInstruction.func = flaggedFunctorMap[reranged / 8];
		currentInstruction.flag = flags[instruction & 0x0f];
	}
	//push and pop instructions 
	else if (instruction < 0xe0) {
		if ((instruction - 0xD8) < 4) currentInstruction.func = push;
		else currentInstruction.func = pop;
		currentInstruction.leftWordOperand = stackOpsMap[instruction & 0x0f];
	}
	//16 bit math
	else if (instruction < 0xEC) {
		currentInstruction.func = wordMathFunctorMap[(instruction - 0xe0) / 4];
		currentInstruction.leftWordOperand = &rHL;
		currentInstruction.rightWordOperand = parameterMap16[instruction&0x0f];
	}
	//16 bit loads
	else if (instruction < 0xF0) {
		currentInstruction.func = ld;
		currentInstruction.leftWordOperand = parameterMap16[instruction & 0x0f];
		currentInstruction.rightWordOperand = &map[++pc];
		pc++;
	}
	//parameterless instructions (test this, the map changed)
	else if (instruction < 0xF9) {
		currentInstruction.func = parameterlessMap[instruction & 0x0f];
	}
	//ld a, *addr
	else if (instruction == 0xfd) {
		currentInstruction.func = ld;
		currentInstruction.leftByteOperand = &rA;
		byte low = map[++pc];
		byte high = map[++pc];
		word address = (high << 8) | low;
		currentInstruction.rightByteOperand = &map[address];
	}
	//ld sp, hl
	else if (instruction == 0xfe) {
		currentInstruction.func = ld;
		currentInstruction.leftByteOperand = &sp; 
		currentInstruction.rightByteOperand = &rHL;
	} 
	//nop
	else {
		currentInstruction.func = nopfunc;
	}
}

byte* renderPointer(int index) {
	if (index == 7 || index == 15) return &map[rHL];
	else return standardParameterMap[index];
}

word* rareParameterMap[] = { &rBC, &rDE, &rHL, &rHL, &rBC, &rDE, &rHL, &rHL };
byte* renderRarePointer(int index) {
	byte* toReturn = &map[*rareParameterMap[index]];
	if(index == 2 || index == 6) rHL++;
	if (index == 3 || index == 7) rHL--;
	return toReturn;
}

void execute() {
	if (currentInstruction.func == ld) load();
	else if (currentInstruction.func == add) addition();
	else if (currentInstruction.func == inc) increment();
	else if (currentInstruction.func == dec) decrement();
	else if (currentInstruction.func == jp) jumpOp();
	else if (currentInstruction.func == nopfunc) printf("nop\n");
	else if (currentInstruction.func == halt) haltOp();
	else {
		printf("Unknown func\n");
	}
}

void nop() {
	//todo, nop
}

void load() {
	if (currentInstruction.leftByteOperand != NULL) {
		*currentInstruction.leftByteOperand = *currentInstruction.rightByteOperand;
	}
	else {
		*currentInstruction.leftWordOperand = *currentInstruction.rightWordOperand;
	}
}

void addition() {
	if (currentInstruction.leftByteOperand != NULL) {
		if (rA + *currentInstruction.rightByteOperand > 255)  setC;
		else resetC;
		rA += *currentInstruction.rightByteOperand;
		if (rA == 0) setZ;
		else resetZ;
	}
	else {
		*currentInstruction.leftWordOperand += *currentInstruction.rightWordOperand;
	}
}

void increment() {
	if (currentInstruction.rightByteOperand != NULL) {
		(*currentInstruction.rightByteOperand)++;
		if (*currentInstruction.rightByteOperand == 0) {
			setZ; setC;
		}
		else {
			resetZ; resetC;
		}
	}
	else {
		(*currentInstruction.rightWordOperand)++;
	}
}

void decrement() {
	if (currentInstruction.rightByteOperand != NULL) {
		(*currentInstruction.rightByteOperand)--;
		if (*currentInstruction.rightByteOperand == 0) setZ; else resetZ;
		if (currentInstruction.rightByteOperand == 0xff) setC; else resetC;
	}
}

void jumpOp() {
	byte lowByte = map[++pc];
	byte highByte = map[++pc];
	if (currentInstruction.flag != none) {
		//hardcoded for now, figure this out
		if (currentInstruction.flag == flagNZ) {
			if ((rF & flagZMask)) {
				printf("Flag z is set! not jumping\n");
				return;
			}
		}
	}
	
	word address = (highByte << 8) | lowByte;
	printf("Flag z not set, jumping to 0x%x\n", address);
	pc = address;
}

void haltOp() { 
	printf("Halting\n");
	halted = 1;
}

