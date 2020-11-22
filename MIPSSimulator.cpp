#include "include.h"


MIPSSimulator::MIPSSimulator(int mode,int numberToExcute)
{
	//only for debug
	//write in data memory
	DMEM[15] = 100;
	this->mode = mode;
	PC = 0;
	for (int i = 0; i < 32; i++)
	{
		if (i >= 8 && i <= 15)
		{
			reg[i] = i;
		}
		else
		{
			reg[i] = 0;
		}
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
			//for debug
			displayStatus();
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
		de.setRS(currentInstr.substr(6, 5));
		de.setRT(currentInstr.substr(11, 5));
		string immField = currentInstr.substr(currentInstr.length() - 16, currentInstr.length());
		de.setImm(signExtend(immField));
		//for debug
		cout << "rs:" << de.getRS() << " rt:" << de.getRT() << " imm:" << de.getImm()<< endl;
	}
	//R-type instrucion
	else
	{
		de.setRS(currentInstr.substr(6,5));
		de.setRT(currentInstr.substr(11, 5));
		de.setRD(currentInstr.substr(16, 5));
		de.setFunc(currentInstr.substr(26, 6));
		//for debug
		cout <<"rs: "<< de.getRS() << "rt: " << de.getRT()<<"rd: "<<de.getRD()<<"func: "<<de.getFunc()<<endl;
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
		//addi
		if (opcode == "001000")
		{
			int rt = BToD(de.getRT());
			em.setRS(de.getRS());
			int result = reg[rt] + BToD(de.getImm());
			em.setALUoutput(result);
		}
		//lw
		if (opcode == "100011")
		{
			//load word format base in rs
			//destination register in rt
			int base = BToD(de.getRS());
			em.setRT(de.getRT());
			int result = reg[base] + BToD(de.getImm());
			em.setALUoutput(result);
		}
	}
	//R-type instruction
	else
	{
		//get rs,rt from DE_EX
		//convert string binary to decimal
		int rs = BToD(de.getRS());
		int rt = BToD(de.getRT());
		em.setRD(de.getRD());
		//add
		if (de.getFunc() == "100000")
		{
			int result = reg[rs] + reg[rt];
			em.setALUoutput(result);
		}
		//sub
		else if (de.getFunc() == "100010")
		{
			int result = reg[rs] - reg[rt];
			em.setALUoutput(result);
		}
		//and
		else if (de.getFunc() == "100100")
		{
			unsigned int and1 = reg[rs];
			unsigned int and2 = reg[rt];
			int result = and1 & and2;
			em.setALUoutput(result);
		}
		//or
		else if (de.getFunc() == "100101")
		{
			unsigned int and1 = reg[rs];
			unsigned int and2 = reg[rt];
			int result = and1 | and2;
			em.setALUoutput(result);
		}
		//sll(shift left logic)
		else if (de.getFunc() == "000000")
		{
			//get shamt from instruction
			//convert string binary to decimal
			int shamt = BToD(de.getIR().substr(21, 5));
			unsigned int source = reg[rt];
			//since << is add zero instead of drop
			//we have to convert it into string and drop first shamt bits
			string temp=DToB(source << shamt);
			string r = temp.substr(temp.length()-5,temp.length());
			int result = BToD(r);
			em.setALUoutput(result);
		}
		//srt(shift right logic)
		else if (de.getFunc() == "000010")
		{
			//same as sll
			int shamt = BToD(de.getIR().substr(21, 5));
			unsigned int source = reg[rt];
			int result = source >> shamt;
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
		mw.setALUoutput(DMEM[em.getALUoutput()]);
		//lw destination register is rt
		mw.setRT(em.getRT());
	}
	else
	{
		mw.setALUoutput(em.getALUoutput());
		mw.setRD(em.getRD());
		//for I-type instructions,
		//destination register is rs
		mw.setRS(em.getRS());
		
	}
}

void MIPSSimulator::writeBack()
{
	string currentInstr = mw.getIR();
	string opcode = currentInstr.substr(0, 6);
	//load word
	if (opcode == "100011")
	{
		int destination = BToD(mw.getRT());
		reg[destination] = mw.getALUoutput();
	}
	//other instructions
	else
	{
		//R-type: destination register is rd
		if (opcode == "000000")
		{
			int destination = BToD(mw.getRD());
			reg[destination] = mw.getALUoutput();
		}
		//I-type: destination register is rs
		else
		{
			int destination = BToD(mw.getRS());
			reg[destination] = mw.getALUoutput();
		}
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

void MIPSSimulator::displayStatus()
{
	for (int i = 8; i < 16; i++)
	{
		cout<<"t"<<i-8<<": " << reg[i] << ' ';
	}
	cout << endl;
}

string MIPSSimulator::signExtend(string immField)
{
	string imm;
	if (immField[0] == '0')
	{
		imm = "0000000000000000" + immField;
	}
	else if (immField[0] == '1')
	{
		imm = "1111111111111111" + immField;
	}
	return imm;
}
