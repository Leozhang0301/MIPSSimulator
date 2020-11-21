#pragma once

class ID_EX
{
public:
	ID_EX();
	~ID_EX();
	void setIR(string IR);
	string getIR();
	void setNPC(int NPC);
	int getNPC();
	void setA(int A);
	int getA();
	void setB(int B);
	int getB();
	void setImm(int imm);
	int getImm();
private:
	string IR;
	int NPC;
	int A;
	int B;
	int imm;
};

