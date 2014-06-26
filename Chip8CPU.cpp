/*
 * Chip8CPU.cpp
 *
 *  Created on: 02-06-2014
 *      Author: Bartłomiej Szcześniak
 */

#include "Chip8CPU.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

Chip8CPU::Chip8CPU() {
	for(int i =0; i < 64*32; i++) gfx[i] = 0;
	for(int i = 0; i <16; i++) {stack[i] = 0; V[i]=0;}
	for(int i = 0; i < 4096; i++) memory[i]=0;
	I = 0;
	sp = 0;
	pc = 0;
	opcode = 0;
	delay_timer = 0;
	sound_timer = 0;
	drawFlag = true;
	srand(time(NULL));
}

Chip8CPU::~Chip8CPU() {
}

void Chip8CPU::fetch()
{
	opcode = ((memory[pc] << 8) + memory[pc+1]);
}

void Chip8CPU::loadFont()
{
	unsigned char chip8_fontset[80] =
	{
	  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	  0x20, 0x60, 0x20, 0x20, 0x70, // 1
	  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	memcpy(memory, chip8_fontset, 80);
}

void Chip8CPU::execute()
{
	fetch();
	(this->*opcodeTable[(opcode & 0xF000) >> 12])();
	pc+=2;
}

bool Chip8CPU::drawReady()
{
	return drawFlag;
}

bool Chip8CPU::isBeep()
{
	return (sound_timer!=0);
}

void Chip8CPU::pushButton(int btn)
{
	key[btn] = 1;
}

void Chip8CPU::releaseButton(int btn)
{
	key[btn] = 0;
}

void Chip8CPU::loadProgram(char* data, int size)
{
	memcpy(memory+512, data, size);
}

void Chip8CPU::loadMemSnapshot(char* data)
{
	memcpy(memory, data, 4096);
}

char* Chip8CPU::getMemSnapshot()
{
	return memory;
}

char* Chip8CPU::getGfx()
{
	return gfx;
}

void Chip8CPU::opNULL()
{
	cerr << "Unimplemented opcode: " << hex << opcode << "\n";
}

void Chip8CPU::opSpec0()
{
	(this->*opSpecial0Table[(opcode & 0x00F0) >> 4])();
}

void Chip8CPU::opClear()
{
	for(int i = 0; i < 64*32; i++) gfx[i] = 0;
}

void Chip8CPU::opReturn()
{
	sp--;
	pc = stack[sp];
	pc-=2;
}

void Chip8CPU::opJump()
{
	pc = (opcode & 0x0FFF);
	pc-=2;
}

void Chip8CPU::opCall()
{
	stack[sp] = pc;
	sp++;
	pc = (opcode & 0x0FFF);
	pc-=2;
}

void Chip8CPU::opEqual()
{
	int x = (opcode >> 8) & 0xF;
	char n = opcode & 0xFF;
	if(V[x] == n) pc+=2;
}

void Chip8CPU::opNEqual()
{
	int x = (opcode >> 8) & 0xF;
	char n = opcode & 0xFF;
	if(V[x] != n) pc+=2;
}

void Chip8CPU::opVEqual()
{
	int x = (opcode >> 8) & 0xF;
	int y = (opcode >> 4) & 0xF;
	if(V[x] == V[y]) pc+=2;
}

void Chip8CPU::opSetV()
{
	int x = (opcode >> 8) & 0xF;
	char n = opcode & 0xFF;
	V[x] = n;
}

void Chip8CPU::opAdd()
{
	int x = (opcode >> 8) & 0xF;
	char n = opcode & 0xFF;
	V[x] += n;
}

void Chip8CPU::opMath()
{
	(this->*opMathTable[(opcode & 0x00F0) >> 4])();
}

void Chip8CPU::opSetVV()
{
	int x = (opcode >> 8) & 0xF;
	int y = (opcode >> 4) & 0xF;
	V[x] = V[y];
}

void Chip8CPU::opOR()
{
	int x = (opcode >> 8) & 0xF;
	int y = (opcode >> 4) & 0xF;
	V[x] = V[x] | V[y];
}

void Chip8CPU::opAND()
{
	int x = (opcode >> 8) & 0xF;
	int y = (opcode >> 4) & 0xF;
	V[x] = V[x] & V[y];
}

void Chip8CPU::opXOR()
{
	int x = (opcode >> 8) & 0xF;
	int y = (opcode >> 4) & 0xF;
	V[x] = V[x] ^ V[y];
}

void Chip8CPU::opVAdd()
{
	int x = (opcode >> 8) & 0xF;
	int y = (opcode >> 4) & 0xF;
	if(V[x] + V[y] > 0xFF)
	{
		V[0xF] = 1;
		V[x] = V[y] + V[x] - 0xFF;
	}
	else
	{
		V[0xF] = 0;
		V[x] = V[y] + V[x];
	}
}

void Chip8CPU::opVSub()
{
	int x = (opcode >> 8) & 0xF;
	int y = (opcode >> 4) & 0xF;
	if(V[x] > V[y]) V[0xF] = 0;
	else V[0xF] = 1;
	V[x] = V[x] - V[y];
}

void Chip8CPU::opVShiftR()
{
	int x = (opcode >> 8) & 0xF;
	V[0xF] = V[x] & 0b1;
	V[x] = V[x] >> 1;
}

void Chip8CPU::opVYSub()
{
	int x = (opcode >> 8) & 0xF;
	int y = (opcode >> 4) & 0xF;
	if(V[y] > V[y]) V[0xF] = 0;
	else V[0xF] = 1;
	V[x] = V[y] - V[x];
}

void Chip8CPU::opVShiftL()
{
	int x = (opcode >> 8) & 0xF;
	V[0xF] = V[x] >> 7;
	V[x] = V[x] << 1;
}

void Chip8CPU::opVNEqual()
{
	int x = (opcode >> 8) & 0xF;
	int y = (opcode >> 4) & 0xF;
	if(V[x] != V[y]) pc+=4;
	else pc+=2;
}

void Chip8CPU::opSetI()
{
	int n = opcode & 0xFFF;
	I = n;
}

void Chip8CPU::opJumpOff()
{
	int n = opcode & 0xFFF;
	pc = n + V[0];
}

void Chip8CPU::opRand()
{
	int x = (opcode >> 8) & 0xF;
	int n = opcode & 0xFF;
	V[x] = rand() & n;
}

void Chip8CPU::opDraw()
{
	int x0 = (opcode >> 8) & 0xF;
	int y0 = (opcode >> 4) & 0xF;
	int h = opcode & 0xF;

	V[0xF] = 0;

	for (int y = 0; y < h; y++)
	{
		int sline = memory[I+y];
		for(int x = 0; x < 8; x++)
		{
			if(sline & (0b10000000 >> x) !=0)
			{
				if(gfx[(y+y0)*64 + x + x0] != 0) V[0xF] = 1;
				gfx[(y+y0)*64 + x + x0] ^= 1;
			}
		}
	}
	drawFlag = true;
}

void Chip8CPU::opKey()
{
	(this->*opKeyTable[opcode & 0x000F])();
}

void Chip8CPU::opKeyXPressed()
{
	int x = (opcode >> 8) & 0xF;
	if(key[V[x]] == true) pc+=2;
}

void Chip8CPU::opKeyXNPressed()
{
	int x = (opcode >> 8) & 0xF;
	if(key[V[x]] == false) pc+=2;
}

void Chip8CPU::opSpecialF()
{
	switch(opcode & 0xFF)
	{
	case 0x07:
		opSetVDelay();
		break;
	case 0x0A:
		opKeyPressWait();
		break;
	case 0x15:
		opSetDelayV();
		break;
	case 0x18:
		opSetSoundV();
		break;
	case 0x1E:
		opIAddV();
		break;
	case 0x29:
		opSetISpriteV();
		break;
	case 0x33:
		opStoreIBCDV();
		break;
	case 0x55:
		opStoreIV();
		break;
	case 0x65:
		opStoreVI();
		break;
	}
}

void Chip8CPU::opSetVDelay()
{
	int x = (opcode >> 8) & 0xF;
	V[x] = delay_timer;
}

void Chip8CPU::opKeyPressWait()
{
	int x = (opcode >> 8) & 0xF;
	for(int i = 0; i < 0xF; i++)
	{
		if(key[i])
		{
			V[x] = i;
			return;
		}
	}
	pc-=2;
}

void Chip8CPU::opSetDelayV()
{
	int x = (opcode >> 8) & 0xF;
	delay_timer = V[x];
}

void Chip8CPU::opSetSoundV()
{
	int x = (opcode >> 8) & 0xF;
	sound_timer = V[x];
}

void Chip8CPU::opIAddV()
{
	int x = (opcode >> 8) & 0xF;
	if(I+V[x] > 0xFFF) V[0xF] = 1;
	I += V[x];
}

void Chip8CPU::opSetISpriteV()
{
	int x = (opcode >> 8) & 0xF;
	I = V[x] * 5; //Sprites are coded binary, with 4x5 size, starting at 0x0
}

void Chip8CPU::opStoreIBCDV()
{
	int x = (opcode >> 8) & 0xF;
	memory[I] = V[x] / 100;
	memory[I+1] = V[x] / 10 % 10;
	memory[I+2] = V[x] % 10;
}

void Chip8CPU::opStoreIV()
{
	int x = (opcode >> 8) & 0xF;
	for(int i = 0; i <= x; i++)
	{
		memory[I+i] = V[i];
	}
}

void Chip8CPU::opStoreVI()
{
	int x = (opcode >> 8) & 0xF;
	for(int i = 0; i <= x; i++)
	{
		V[i] = memory[I+i];
	}
}
