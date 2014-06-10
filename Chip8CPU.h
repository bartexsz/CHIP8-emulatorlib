/*
 * Chip8CPU.h
 *
 *  Created on: 02-06-2014
 *      Author: Bartłomiej Szcześniak
 */

#ifndef CHIP8CPU_H_
#define CHIP8CPU_H_

class Chip8CPU {
private:
	char gfx[64*32];
	char memory[4096];
	char V[16];
	short stack[16];
	short I;
	short sp;
	short pc;
	short opcode;
	int delay_timer;
	int sound_timer;
	bool drawFlag;
	int state;
	void fetch();
	void (Chip8CPU::*opcodeTable[16])() =
	{
			&Chip8CPU::opSpec0, &Chip8CPU::opJump, &Chip8CPU::opCall,
			&Chip8CPU::opEqual, &Chip8CPU::opNEqual, &Chip8CPU::opVEqual,
			&Chip8CPU::opSetV, &Chip8CPU::opAdd, &Chip8CPU::opMath,
			&Chip8CPU::opVNEqual, &Chip8CPU::opSetI, &Chip8CPU::opJumpOff,
			&Chip8CPU::opRand, &Chip8CPU::opNULL, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL
	};
	void (Chip8CPU::*opSpecial0Table[16])() =
	{
			&Chip8CPU::opClear, &Chip8CPU::opNULL, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL, &Chip8CPU::opNULL, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL, &Chip8CPU::opNULL, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL, &Chip8CPU::opNULL, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL, &Chip8CPU::opNULL, &Chip8CPU::opReturn,
			&Chip8CPU::opNULL
	};
	void (Chip8CPU::*opMathTable[16])() =
	{
			&Chip8CPU::opSetVV, &Chip8CPU::opOR, &Chip8CPU::opAND,
			&Chip8CPU::opXOR, &Chip8CPU::opVAdd, &Chip8CPU::opVSub,
			&Chip8CPU::opVShiftR, &Chip8CPU::opVYSub, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL, &Chip8CPU::opNULL, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL, &Chip8CPU::opNULL, &Chip8CPU::opVShiftL,
			&Chip8CPU::opNULL
	};

	void opNULL(); //Unimplemented opcode

	void opSpec0(); //Special System opcodes starting with 0
	void opClear(); //opcode 00E0 - clear the display
	void opReturn(); //opcode 00EE - return from subroutine

	void opJump(); //opcode 1NNN - jump to NNN
	void opCall(); //opcode 2NNN - call subroutine
	void opEqual(); //opcode 3XNN - skip if V[x] equals NN
	void opNEqual(); //opcode 4XNN -skip if V[x] not equals NN
	void opVEqual(); //opcode 5XY0 - skip if V[x] equal V[y]
	void opSetV(); //opcode 6XNN - set V[x] to NN
	void opAdd(); //opcode 7XNN - add NN to V[x]

	void opMath(); //math opcodes starting with 8
	void opSetVV(); //opcode 8XY0 - set V[x] to V[y]
	void opOR(); //opcode 8XY1 - V[x] = V[x] OR V[y]
	void opAND(); //opcode 8XY2 - V[x] = V[x] AND V[y]
	void opXOR(); //opcode 8XY3 - V[x] = V[x] XOR V[y]
	void opVAdd(); //opcode 8XY4 - V[x] = V[x] + V[y], V[F] set if there is a carry
	void opVSub(); //opcode 8XY5 - V[x] = V[x] - V[y], V[F] 0 set if there is a borrow
	void opVShiftR(); //opcode 8XY6 - V[F] - least significant bit of V[x], shifts V[x] 1 bit right
	void opVYSub(); //opcode 8XY7 - V[x] = V[y] - V[x], V[F] set 0 if there is a borrow
	void opVShiftL(); //opcode 8XYE - V[F] - most significant bit of V[x], shifts V[x] 1 bit left

	void opVNEqual(); //opcode 9xy0 - skip if V[x] not equal V[y]
	void opSetI(); //opcode ANNN - set I to NNN
	void opJumpOff(); //opcode BNNN - jumps to NNN + V[0]
	void opRand(); //opcode CxNN - sets V[x] to random integer & NN

public:
	Chip8CPU();
	virtual ~Chip8CPU();
	void update(char* keypad);
	char* getGfx();
	bool drawReady();
	int getState();
	void setState(int state);
	void execute();
};

#endif /* CHIP8CPU_H_ */
