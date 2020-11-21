#include "include.h"


MIPSSimulator::MIPSSimulator(int mode,int numberToExcute)
{
	this->mode = mode;
	PC = 0;
	for (int i = 0; i < 32; i++)
	{
		reg[i] = 0;
	}
	cycleClk = 0;
	this->numberToExcute = numberToExcute;
}

void MIPSSimulator::process()
{
	//instruction by instruction
	if (mode == 1)
	{
		while (numberToExcute > 0)
		{
			fetch();
			cycleClk++;
			decode();
			cycleClk++;
			excute();
			cycleClk++;
			memory();
			cycleClk++;
			writeBack();
			cycleClk++;
			numberToExcute--;
		}
	}
	//cycle by cycle
	else
	{

	}
}

MIPSSimulator::~MIPSSimulator()
{
	
}

void MIPSSimulator::fetch()
{
	//get instr in pc into fd
	fd.setIR(IMEM[PC]);
	string currentInstr = fd.getIR();
	string opcode = currentInstr.substr(0,6);
	//if not branch
	if (opcode != "101011")
	{
		PC++;
	}
	//branch
	else
	{

	}
}

void MIPSSimulator::decode()
{
	//get instruction from IF_ID
	de.setIR(fd.getIR());
	string currentInstr = fd.getIR();
	//get opcode
	string opcode = currentInstr.substr(0, 6);
	//I-type instruction
	if (opcode != "000000")
	{

	}
	//R-type instrucion
	else
	{
		de.setRS(currentInstr.substr(6,5));
		de.setRT(currentInstr.substr(11, 5));
		de.setRD(currentInstr.substr(16, 5));
		de.setFunc(currentInstr.substr(26, 6));
		cout << de.getRS() << ' ' << de.getRT()<<' '<<de.getRD()<<' '<<de.getFunc();
	}
}

void MIPSSimulator::excute()
{
	em.setIR(de.getIR());
	string currentInstr = de.getIR();
	string opcode = currentInstr.substr(0, 6);
	//I-type instruction
	if (opcode != "000000")
	{

	}
	//R-type instruction
	else
	{
		
		//get rs,rt,rd from EX_ME
		int rs = BToD(de.getRS());
		int rt = BToD(de.getRT());
		em.setRD(de.getRD());
		//add
		if (de.getFunc() == "100000")
		{
			int result = reg[rs] + reg[rt];
			em.setALUoutput(result);
		}
	}
}

void MIPSSimulator::memory()
{
	mw.setIR(em.getIR());
	string currentInstr = em.getIR();
	string opcode = currentInstr.substr(0, 6);
	//load word and store word
	if (opcode == "100011" || opcode == "101011")
	{

	}
	else
	{
		mw.setALUoutput(em.getALUoutput());
		mw.setRD(em.getRD());
	}
}

void MIPSSimulator::writeBack()
{
	string currentInstr = mw.getIR();
	string opcode = currentInstr.substr(0, 6);
	//store word
	if (opcode == "101011")
	{

	}
	//other instructions
	else
	{
		int destination = BToD(mw.getRD());
		reg[destination] = mw.getALUoutput();
	}
}

int MIPSSimulator::BToD(string n)
{
	long number = stoi(n);
	int i = 0;
	int decimalNumber = 0;
	int remainder;
	while (number != 0)
	{
		remainder = number % 10;
		number /= 10;
		decimalNumber += remainder * pow(2, i);
		++i;
	}
	return decimalNumber;
}

string MIPSSimulator::DToB(int n)
{
	int binaryNumber = 0;
	int step = 1;
	int i = 1;
	int remainder;
	while (n != 0)
	{
		remainder = n % 2;
		n /= 2;
		binaryNumber += remainder * i;
		i *= 10;
	}
	return to_string(binaryNumber);
}
