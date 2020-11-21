#include "include.h"


MIPSSimulator::MIPSSimulator(int mode,int numberToExcute)
{
	this->mode = mode;
	PC = 0;
	reg[32]={0};
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
	
}

void MIPSSimulator::decode()
{
}

void MIPSSimulator::excute()
{
}

void MIPSSimulator::memory()
{
}

void MIPSSimulator::writeBack()
{
}
