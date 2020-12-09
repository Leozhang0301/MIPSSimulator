#include "include.h"
#include <string>
using namespace std;

IF_ID::IF_ID()
{
	IR = "";
	NPC = 0;
}

IF_ID::~IF_ID()
{
}

void IF_ID::setIR(string IR)
{
	this->IR = IR;
}

string IF_ID::getIR()
{
	return this->IR;
}

void IF_ID::setNPC(int NPC)
{
	this->NPC=NPC;
}

int IF_ID::getNPC()
{
	return this->NPC;
}

void IF_ID::display()
{
	cout << "fetch_decode latch conten:" << endl;
	cout << "the current instruction is " << getIR() << endl;
	cout << "the program counter is " << getNPC() << endl;
}
