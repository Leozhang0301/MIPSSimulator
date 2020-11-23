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
	PC++;
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
		//sign extend
		if (opcode != "001100" && opcode!="001101" && opcode!="001011")
		{
			de.setImm(signExtend(immField));
		}
		//addi zero extend 
		else
		{
			de.setImm(zeroExtend(immField));
		}
		//only for debug
		cout << "rs:" << de.getRS() << " rt:" << de.getRT() << " imm:" << de.getImm()<< endl;
	}
	//R-type instrucion
	else
	{
		de.setRS(currentInstr.substr(6,5));
		de.setRT(currentInstr.substr(11, 5));
		de.setRD(currentInstr.substr(16, 5));
		de.setFunc(currentInstr.substr(26, 6));
		//only for debug
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
		else if (opcode == "100011"||opcode=="101011")
		{
			//load word format base in rs
			//destination register in rt
			int base = BToD(de.getRS());
			em.setRT(de.getRT());
			int result = reg[base] + BToD(de.getImm());
			em.setALUoutput(result);
		}
		//beq
		else if (opcode == "000100")
		{
			int rs = BToD(de.getRS());
			int rt = BToD(de.getRT());
			if (reg[rs] == reg[rt])
			{
				//EX/MEM.ALUOutput <-PC + (ID/EX.Imm <<2);
				int result =BToD(de.getImm());
				em.setALUoutput(result);
				PC += result;
			}	
		}
		//lui
		else if (opcode == "001111")
		{
			em.setRT(de.getRT());
			//since when decode stage imm sign extended
			//get substring and add 16's 0 followed
			string imm = de.getImm();
			string content = imm.substr(imm.length() - 16, imm.length())+"0000000000000000";
			int result = BToD(content);
			em.setALUoutput(result);
		}
		//andi
		else if (opcode == "001100")
		{
			int rt = BToD(de.getRT());
			em.setRS(de.getRS());
			int result = reg[rt] & BToD(de.getImm());
			em.setALUoutput(result);
		}
		//ori
		else if (opcode == "001101")
		{
			int rt = BToD(de.getRT());
			em.setRS(de.getRS());
			int result = reg[rt] | BToD(de.getImm());
			em.setALUoutput(result);
		}
		//slti and sltiu
		else if (opcode == "001010"||opcode=="001011")
		{
			int rt = BToD(de.getRT());
			em.setRS(de.getRS());
			int result;
			if (reg[rt] < BToD(de.getImm()))
			{
				result=1;
			}
			else
			{
				result=0;
			}
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
		//mul
		else if (de.getFunc() == "010010")
		{
			int result = reg[rs] * reg[rt];
			em.setALUoutput(result);
		}
	}
}

void MIPSSimulator::memory()
{
	mw.setIR(em.getIR());
	string currentInstr = em.getIR();
	string opcode = currentInstr.substr(0, 6);
	//load word
	if (opcode == "100011")
	{
		mw.setALUoutput(DMEM[em.getALUoutput()]);
		//lw destination register is rt
		mw.setRT(em.getRT());
	}
	//store word
	else if (opcode == "101011")
	{
		DMEM[em.getALUoutput()] = reg[BToD(em.getRT())];
	}
	else
	{
		mw.setALUoutput(em.getALUoutput());
		mw.setRD(em.getRD());
		//for I-type instructions,
		//destination register is rs
		mw.setRS(em.getRS());
		mw.setRT(em.getRT());
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
			//if instruction is mul lower write into rd and higher write into rd+1
			if (currentInstr.substr(26, 6) == "010010")
			{
				int destination = BToD(mw.getRD());
				string result = DToB(mw.getALUoutput());
				//extend result into 64 bits
				if (result.length() < 64)
				{
					string temp;
					int s = 64 - result.length();
					for (int i = 0; i < s; i++)
					{
						temp += '0';
					}
					temp += result;
					result =temp;
				}
				string lower = result.substr(32, 32);
				string upper = result.substr(0, 32);
				reg[destination] = BToD(lower);
				reg[destination + 1] = BToD(upper);
			}
			else
			{
				int destination = BToD(mw.getRD());
				reg[destination] = mw.getALUoutput();
			}
			
		}
		//sw, beq do nothing
		else if(opcode=="101011"|| opcode == "000100")
		{
			//only for debug
			cout << DMEM[em.getALUoutput()];
		}
		//I-type: destination register is rs
		else
		{
			//lui destination register is rt
			if (opcode == "001111")
			{
				int destination = BToD(mw.getRT());
				reg[destination] = mw.getALUoutput();
			}
			else
			{
				int destination = BToD(mw.getRS());
				reg[destination] = mw.getALUoutput();
			}
		}
		
	}
}

int MIPSSimulator::BToD(string n)
{
	int decimalNumber = 0;
	for (int i = 0; i < n.length(); i++)
	{
		if (n[n.length() - 1 - i] == '1')
		{
			decimalNumber += pow(2, i);
		}
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

string MIPSSimulator::zeroExtend(string immField)
{
	string imm;
	imm += "0000000000000000" + immField;
	return imm;
}
