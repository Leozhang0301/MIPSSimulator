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
private:
	string IR;
	int ALUoutput;
	int LMD;
};

