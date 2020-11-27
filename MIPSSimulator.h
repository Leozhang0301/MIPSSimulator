#pragma once
class MIPSSimulator
{
public:
	MIPSSimulator(int mode,int instrToExecute);
	void process();
	void setInstrToExe(int num);
	void setInstr(int i);
	void setCycle(int i);
	~MIPSSimulator();
private:
	int instrToExecute;
	int cycleClk;
	int cycleToExecute;
	int instr;
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
	int BToD(string n);//convert binary to decimal
	string DToB(int n);//convert decimal to binary
	void displayStatus();
	string signExtend(string immField);
	string zeroExtend(string immField);
};

