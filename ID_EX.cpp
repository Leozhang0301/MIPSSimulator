#include "include.h"

ID_EX::ID_EX()
{
	IR = "";
	NPC = 0;
	RS = "";
	RT = "";
	imm = "";
	cond = 0;
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

void ID_EX::setRS(string RS)
{
	this->RS = RS;
}

string ID_EX::getRS()
{
	return this->RS;
}

void ID_EX::setRT(string RT)
{
	this->RT = RT;
}

string ID_EX::getRT()
{
	return this->RT;
}

void ID_EX::setImm(string imm)
{
	this->imm = imm;
}

string ID_EX::getImm()
{
	return this->imm;
}

void ID_EX::setRD(string RD)
{
	this->RD = RD;
}

string ID_EX::getRD()
{
	return this->RD;
}

void ID_EX::setFunc(string func)
{
	this->func = func;
}

string ID_EX::getFunc()
{
	return this->func;
}

void ID_EX::setCond(int cond)
{
	this->cond = cond;
}

int ID_EX::getCond()
{
	return this->cond;
}

void ID_EX::display()
{
	cout << "decode_execute latch conten:" << endl;
	cout << "the current instruction is " << getIR() << endl;
	cout << "the source register is " << getRS() << endl;
	cout << "the temp register is " << getRT() << endl;
	cout << "the destination register is" << getRD() << endl;
	cout << "the function field is " << getFunc() << endl;
	cout << "the immediate field is " << getImm() << endl;
}
