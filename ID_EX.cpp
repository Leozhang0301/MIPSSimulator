#include "include.h"

ID_EX::ID_EX()
{
	IR = "";
	NPC = 0;
	A = 0;
	B = 0;
	imm = 0;
}

ID_EX::~ID_EX()
{

}

void ID_EX::setIR(string IR)
{
	this->IR = IR;
}

string ID_EX::getIR()
{
	return this->IR;
}

void ID_EX::setNPC(int NPC)
{
	this->NPC = NPC;
}

int ID_EX::getNPC()
{
	return this->NPC;
}

void ID_EX::setA(int A)
{
	this->A = A;
}

int ID_EX::getA()
{
	return this->A;
}

void ID_EX::setB(int B)
{
	this->B = B;
}

int ID_EX::getB()
{
	return this->B;
}

void ID_EX::setImm(int imm)
{
	this->imm = imm;
}

int ID_EX::getImm()
{
	return this->imm;
}
