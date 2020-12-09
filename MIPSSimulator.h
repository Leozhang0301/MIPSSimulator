#pragma once
class MIPSSimulator
{
public:
	MIPSSimulator(int mode,int instrToExecute);
	void process();
	void setInstrToExe(int num);
	void setInstr(int i);
	void setCycle(int i);
	double utilization(int stage);
	~MIPSSimulator();
private:
	int instrToExecute;
	double cycleClk;
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
	string DToH(int n);//convert decimal to hex, use for displaying
	void displayStatus(int opcode);
	string signExtend(string immField);
	string zeroExtend(string immField);

	bool isDataHazard;
	double fetchStall;
	double decodeStall;
	double executeStall;
	double memoryStall;
	double writeBackStall;
};

