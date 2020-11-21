#include "include.h"

EX_ME::EX_ME()
{
	IR = "";
	B = 0;
	ALUoutput = 0;
	cond = 0;
}

EX_ME::~EX_ME()
{

}

void EX_ME::setIR(string IR)
{
	this->IR = IR;
}

string EX_ME::getIR()
{
	return this->IR;
}

void EX_ME::setALUoutput(int ALUoutput)
{
	this->ALUoutput = ALUoutput;
}

int EX_ME::getALUoutput()
{
	return this->ALUoutput;
}

void EX_ME::setcond(int cond)
{
	this->cond = cond;
}

int EX_ME::getcond()
{
	return this->cond;
}

void EX_ME::setB(int B)
{
	this->B = B;
}

int EX_ME::getB()
{
	return this->B;
}
