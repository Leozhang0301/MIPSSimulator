#include "include.h"

ID_EX::ID_EX()
{
	IR = "";
	NPC = 0;
	RS = "";
	RT = "";
	imm = "";
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
