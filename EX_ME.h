#pragma once

class EX_ME
{
public:
	EX_ME();
	~EX_ME();
	void setIR(string IR);
	string getIR();
	void setALUoutput(int NPC);
	int getALUoutput();
	void setcond(int cond);
	int getcond();
	void setFunc(string func);
	string getFunc();
	void setRS(string RS);
	string getRS();
	void setRT(string RT);
	string getRT();
	void setRD(string RD);
	string getRD();
	void display();//display latch content
private:
	string IR;
	int ALUoutput;
	int cond;//branch flag
	string func;
	string RS;//source1 register
	string RT;//source2 register
	string RD;//destination register
	
};

