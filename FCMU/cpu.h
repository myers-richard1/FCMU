#pragma once

#include "Types.h"

word sp; //stack pointer
word pc; //program counter

Register regAF, regBC, regDE, regHL;


void init_cpu();
void test_cpu();

void cycle();

void decode(byte instruction);
byte* renderPointer(int index);
byte* renderRarePointer(int index);
void execute();
int halted;

//remember, the dst/src are not values, but parameter IDs
void nop();
void addition();
void subtraction();
void bitwiseAnd();
void bitwiseOr();
void bitwiseXor();
void increment();
void decrement();

void compare();
void pushRegister();
void popRegister();
void load();
void bitCheck();
void swapA();
void shiftRight();
void shiftLeft();
void rotateRight();
void rotateLeft();
void complement();

void jumpOp();
void returnOp();
void callOp();
void eiOp();
void diOp();
void bcdOp();
void haltOp();
