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
	void setB(int B);
	int getB();
private:
	string IR;
	int B;
	int ALUoutput;
	int cond;
};

