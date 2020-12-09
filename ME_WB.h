#pragma once
#include <string>
using namespace std;
class ME_WB
{
public:
	void setIR(string IR);
	string getIR();
	void setALUoutput(int ALUoutput);
	int getALUoutput();
	void setLMD(int LMD);
	int getLMD();
	void setRD(string RD);
	string getRD();
	void setRS(string RS);
	string getRS();
	void setRT(string RT);
	string getRT();
	void display();//display latch content
private:
	string IR;
	int ALUoutput;
	int LMD;
	string RD;
	string RS;
	string RT;
	
};

