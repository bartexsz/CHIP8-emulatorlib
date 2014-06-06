/*
 * Chip8CPU.cpp
 *
 *  Created on: 02-06-2014
 *      Author: Bartłomiej Szcześniak
 */

#include "Chip8CPU.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Chip8CPU::Chip8CPU() {
	state = 1;
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

void Chip8CPU::execute()
{
	fetch();
	(this->*opcodeTable[(opcode & 0xF000) >> 12])();
}

void Chip8CPU::opNULL()
{
	cerr << "Unimplemented opcode: " << hex << opcode << "\n";
}

void Chip8CPU::opJump()
{
	pc = (opcode & 0x0FFF);
}

void Chip8CPU::opCall()
{
	stack[sp] = pc;
	sp++;
	pc = (opcode & 0x0FFF);
}

void Chip8CPU::opEqual()
{
	int x = (opcode >> 8) & 0xF;
	char n = opcode & 0xFF;
	if(V[x] == n) pc+=4;
	else pc+=2;
}

void Chip8CPU::opNEqual()
{
	int x = (opcode >> 8) & 0xF;
	char n = opcode & 0xFF;
	if(V[x] != n) pc+=4;
	else pc+=2;
}

void Chip8CPU::opVEqual()
{
	int x = (opcode >> 8) & 0xF;
	int y = (opcode >> 4) & 0xF;
	if(V[x] == V[y]) pc+=4;
	else pc+=2;
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
