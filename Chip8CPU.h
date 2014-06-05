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
			&Chip8CPU::opEqual, &Chip8CPU::opNULL, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL, &Chip8CPU::opNULL, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL, &Chip8CPU::opNULL, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL, &Chip8CPU::opNULL, &Chip8CPU::opNULL,
			&Chip8CPU::opNULL
	};

	void opNULL();
	void opJump();
	void opCall();
	void opEqual();

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
