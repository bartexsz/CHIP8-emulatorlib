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
			&Chip8CPU::opNULL, &Chip8CPU::opJump, &Chip8CPU::opCall,
			&Chip8CPU::opEqual, &Chip8CPU::opNEqual, &Chip8CPU::opVEqual,
			&Chip8CPU::opSetV, &Chip8CPU::opNULL, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL, &Chip8CPU::opNULL, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL, &Chip8CPU::opNULL, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL
	};

	void opNULL(); //Unimplemented opcode
	void opJump(); //opcode 1NNN - jump
	void opCall(); //opcode 2NNN - call subroutine
	void opEqual(); //opcode 3XNN - skip if V[x] equals NN
	void opNEqual(); //opcode 4XNN -skip if V[x] not equals NN
	void opVEqual(); //opcode 5XY0 - skip if V[x] equal V[y]
	void opSetV(); //opcode 6XNN - sets V[x] to NN

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
