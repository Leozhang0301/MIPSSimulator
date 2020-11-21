#pragma once
class MIPSSimulator
{
public:
	MIPSSimulator(int mode,int numberToExcute);
	void process();
	~MIPSSimulator();
private:
	int numberToExcute;
	int cycleClk;
	int reg[32];
	IF_ID fd;
	ID_EX de;
	EX_ME em;
	ME_WB mw;
	int mode;
	int PC;
	void fetch();
	void decode();
	void excute();
	void memory();
	void writeBack();
};
