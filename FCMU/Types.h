#pragma once

typedef unsigned char byte;
typedef unsigned short int word;

typedef enum functor {
	ld, add, adc, sub, sbc, inc, dec, cmp, 
	and, or, xor, bit, jp, call, ret,
	push, pop, ei, di, halt, swap,
	cpl, bcd, nopfunc
} functor;

typedef enum Flag {
	none,
	flagZ, flagN, flagH, flagC,
	flagNZ, flagNN, flagNH, flagNC
} Flag;

typedef struct Instruction {
	functor func;
	byte* leftByteOperand;
	byte* rightByteOperand;
	word* leftWordOperand;
	word* rightWordOperand;
	Flag flag;
} Instruction;

typedef union Register {
	word reg16;
	struct {
		byte low;
		byte high;
	};
} Register;


#define rA regAF.high
#define rF regAF.low
#define rB regBC.high
#define rC regBC.low
#define rD regDE.high
#define rE regDE.low
#define rH regHL.high
#define rL regHL.low

#define rAF regAF.reg16
#define rBC regBC.reg16
#define rDE regDE.reg16
#define rHL regHL.reg16

#define flagZMask 0b10000000
#define flagCMask 0b01000000

#define setZ rF |= flagZMask
#define resetZ rF &= ~(flagZMask)
#define setC rF |= flagCMask
#define resetC rF &= ~(flagCMask)
