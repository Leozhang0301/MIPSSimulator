#include "include.h"

void ME_WB::setIR(string IR)
{
	this->IR = IR;
}

string ME_WB::getIR()
{
	return this->IR;
}

void ME_WB::setALUoutput(int ALUoutput)
{
	this->ALUoutput = ALUoutput;
}

int ME_WB::getALUoutput()
{
	return this->ALUoutput;
}

void ME_WB::setLMD(int LMD)
{
	this->LMD = LMD;
}

int ME_WB::getLMD()
{
	return LMD;
}

void ME_WB::setRD(string RD)
{
	this->RD = RD;
}

string ME_WB::getRD()
{
	return this->RD;
}

void ME_WB::setRS(string RS)
{
	this->RS = RS;
}

string ME_WB::getRS()
{
	return this->RS;
}

void ME_WB::setRT(string RT)
{
	this->RT = RT;
}

string ME_WB::getRT()
{
	return this->RT;
}

void ME_WB::display()
{
	cout << "memory_writeback latch conten:" << endl;
	cout << "the current instruction is " << getIR() << endl;
	cout << "the source register is " << getRS() << endl;
	cout << "the temp register is " << getRT() << endl;
	cout << "the destination register is" << getRD() << endl;
	cout << "the ALU output is" << getALUoutput() << endl;
}
