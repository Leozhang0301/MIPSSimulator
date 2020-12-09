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
	void setRS(string RS);
	string getRS();
	void setRT(string RT);
	string getRT();
	void setImm(string imm);
	string getImm();
	void setRD(string RD);
	string getRD();
	void setFunc(string func);
	string getFunc();
	void setCond(int cond);
	int getCond();
	void display();//display latch content
private:
	string IR;//instruction
	int NPC;//program counter
	string RS;//source1 register
	string RT;//source2 register
	string RD;//destination register
	string imm;//immediate
	string func;//operate function
	int cond;
	
};

