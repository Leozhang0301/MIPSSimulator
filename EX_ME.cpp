#include "include.h"

EX_ME::EX_ME()
{
	IR = "";
	RT = "";
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

void EX_ME::setRS(string RS)
{
	this->RS = RS;
}

string EX_ME::getRS()
{
	return this->RS;
}

void EX_ME::setRT(string RT)
{
	this->RT = RT;
}

string EX_ME::getRT()
{
	return this->RT;
}

void EX_ME::setFunc(string func)
{
	this->func = func;
}

string EX_ME::getFunc()
{
	return this->func;
}

void EX_ME::setRD(string RD)
{
	this->RD = RD;
}

string EX_ME::getRD()
{
	return this->RD;
}

void EX_ME::display()
{
	cout << "execute_memory latch conten:" << endl;
	cout << "the current instruction is " << getIR() << endl;
	cout << "the source register is " << getRS() << endl;
	cout << "the temp register is " << getRT() << endl;
	cout << "the destination register is" << getRD() << endl;
	cout << "the function field is " << getFunc() << endl;
	cout << "the ALU output is" << getALUoutput() << endl;
}
