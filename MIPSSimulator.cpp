#include "include.h"


MIPSSimulator::MIPSSimulator(int mode,int numberToExecute)
{
	this->mode = mode;
	PC = 0;
	//only for debug
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
	if (mode == 1)
	{
		this->instrToExecute = numberToExecute;
	}
	else if (mode == 2)
	{
		this->cycleToExecute = numberToExecute;
	}
	instr = 0;
	isDataHazard = false;
	fetchStall = 0;
	decodeStall = 0;
	memoryStall = 0;
	executeStall = 0;
	writeBackStall = 0;
}

void MIPSSimulator::process()
{
	//instruction by instruction
	if (mode == 1)
	{
		while (instr!=instrToExecute)
		{	
			//reverse order dont need temp latch
			writeBack();
			memory();
			excute();
			decode();
			fetch();
			cycleClk++;
		}
	}
	//cycle by cycle
	else
	{
		while (cycleClk != cycleToExecute)
		{
			//reverse order dont need temp latch
			writeBack();
			memory();
			excute();
			decode();
			fetch();
			//for debug
			displayStatus(mode);
			cycleClk++;
		}
	}
}

void MIPSSimulator::setInstrToExe(int num)
{
	this->instrToExecute = num;
}

void MIPSSimulator::setInstr(int i)
{
	this->instr = i;
}

void MIPSSimulator::setCycle(int i)
{
	this->cycleClk = i;
}

MIPSSimulator::~MIPSSimulator()
{
	
}

void MIPSSimulator::fetch()
{
	//get instr in pc into fd
	fd.setIR(IMEM[PC]);
	string currentInstr = fd.getIR();
	if (currentInstr != "")
	{
		
		//if previous instruction is branch
		if (de.getCond()==1)
		{
			cout << "previous instrucion is branch" << endl;
		}
		//if previous instruction is not branch
		else if (de.getCond() == 0)
		{
			//if rs==rt
			if (em.getcond() == 1)
			{
				//reset flag
				em.setcond(0);
				PC = em.getALUoutput();
				fd.setIR(IMEM[PC]);
			}
			else
			{
				PC++;
			}
		}
	}
	else
	{
		fetchStall++;
		//cout << "fetch stall" << endl;
	}
}

void MIPSSimulator::decode()
{
	//get instruction from IF_ID
	de.setIR(fd.getIR());
	string currentInstr = fd.getIR();
	if (currentInstr != ""&& !isDataHazard)
	{
		//get opcode
		string opcode = currentInstr.substr(0, 6);
		//I-type instruction
		if (opcode != "000000")
		{
			de.setRS(currentInstr.substr(6, 5));
			de.setRT(currentInstr.substr(11, 5));
			string immField = currentInstr.substr(currentInstr.length() - 16, currentInstr.length());
			//addi lw sw beq slti sign extend
			if (opcode == "001000" || opcode == "100011" || opcode == "101011"||opcode=="000100"||opcode=="001010")
			{
				//if instr is beq
				if (opcode == "000100")
				{
					//set flag as 1
					de.setCond(1);
				}
				de.setImm(signExtend(immField));
			}
			//andi ori sltiu zero extend 
			else
			{
				de.setImm(zeroExtend(immField));
			}
			//if beq instruction both rs & rt are source
			if (opcode == "000100")
			{
				if (de.getRS() == em.getRT() || de.getRT() == em.getRT())
				{
					de.setIR("");
					PC--;
					isDataHazard = true;
				}
			}
			//if not beq only rs is source
			else
			{
				//if rs equal EX_ME destination
				if (de.getRS() == em.getRT())
				{
					de.setIR("");
					PC--;
					isDataHazard = true;
				}
			}
			//only for debug
			//cout << "rs:" << de.getRS() << " rt:" << de.getRT() << " imm:" << de.getImm() << endl;
		}
		//R-type instrucion
		else
		{
			
			de.setRS(currentInstr.substr(6, 5));
			de.setRT(currentInstr.substr(11, 5));
			de.setRD(currentInstr.substr(16, 5));
			de.setFunc(currentInstr.substr(26, 6));
			//if source equal EX_ME destination
			if (de.getRS() == em.getRD()||de.getRT()==em.getRD())
			{
				//execute stall
				de.setIR("");
				PC--;
				isDataHazard = true;
			}
			//only for debug
			//cout << "rs:" << de.getRS() << " rt:" << de.getRT() << " rd:" << de.getRD() << " func:" << de.getFunc() << endl;
		}
	}
	else
	{
		decodeStall++;
		//cout << "decode stall" << endl;
	}
}

void MIPSSimulator::excute()
{
	em.setIR(de.getIR());
	string currentInstr = de.getIR();
	if(currentInstr!="")
	{
		string opcode = currentInstr.substr(0, 6);
		//I-type instruction
		if (opcode != "000000")
		{
			//addi
			if (opcode == "001000")
			{
				int rs = BToD(de.getRS());
				em.setRT(de.getRT());
				int result = reg[rs] + BToD(de.getImm());
				em.setALUoutput(result);
			}
			//lw
			else if (opcode == "100011" || opcode == "101011")
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
				//set de flag as 0 enable fetch
				de.setCond(0);
				fd.setIR("");
				int rs = BToD(de.getRS());
				int rt = BToD(de.getRT());
				if (reg[rs] == reg[rt])
				{
					//EX/MEM.ALUOutput <-PC + (ID/EX.Imm <<2);
					int result = BToD(de.getImm());
					em.setALUoutput(result+1);
					//set em flag as 1 remain previous instruction is branch
					em.setcond(1);
				}
			}
			//lui
			else if (opcode == "001111")
			{
				em.setRT(de.getRT());
				//since when decode stage imm sign extended
				//get substring and add 16's 0 followed
				string imm = de.getImm();
				string content = imm.substr(imm.length() - 16, imm.length()) + "0000000000000000";
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
			else if (opcode == "001010" || opcode == "001011")
			{
				int rt = BToD(de.getRT());
				em.setRS(de.getRS());
				int result;
				if (reg[rt] < BToD(de.getImm()))
				{
					result = 1;
				}
				else
				{
					result = 0;
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
				string temp = DToB(source << shamt);
				string r = temp.substr(temp.length() - 5, temp.length());
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
	else
	{
	    executeStall++;
		//cout << "execute stall" << endl;
	}
}

void MIPSSimulator::memory()
{
	mw.setIR(em.getIR());
	string currentInstr = em.getIR();
	if (currentInstr != "")
	{
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
			cout <<"The content in regester "<< BToD(em.getRT())<<" has been stored in memory "<< em.getALUoutput()<<endl;
			cout << "Content is " << reg[BToD(em.getRT())] << endl;
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
	else
	{
		memoryStall++;
		//cout << "memory stall" << endl;
	}
}

void MIPSSimulator::writeBack()
{
	string currentInstr = mw.getIR();
	if (currentInstr != "")
	{
		//reset data hazard
		if (isDataHazard)
		{
			isDataHazard = false;
		}
		string opcode = currentInstr.substr(0, 6);
		//load word
		if (opcode == "100011")
		{
			int destination = BToD(mw.getRT());
			reg[destination] = mw.getALUoutput();
			cout << "load word from memory secussed" << endl;
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
						result = temp;
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
			else if (opcode == "101011" || opcode == "000100")
			{
				//empty
			}
			//I-type: destination register is rs
			else
			{
				//lui&addi destination register is rt
				if (opcode == "001111"||opcode=="001000")
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
		//instruction number add 1
		instr++;
		//for debug
		if (mode != 2)
		{
			displayStatus(mode);
		}
	}
	else
	{
		writeBackStall++;
		//cout << "write back stall" << endl;
	}
}

//convert binary string to decimal
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

//convert decimal to binary string
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

//display status registers content
void MIPSSimulator::displayStatus(int mode)
{
	cout << "***********************" << endl;
	switch (mode)
	{
	case 1:
		for (int i = 8; i < 16; i++)
		{
			string temp = DToH(reg[i]);
			if (temp == "")
			{
				cout << "t" << i - 8 << ": 0x0000" << ' ';
			}
			else
			{
				cout << "t" << i - 8 << ": 0x" << temp << ' ';
			}
		}
		cout << endl;
		break;
	case 2:
		fd.display();
		cout << endl;
		de.display();
		cout << endl;
		em.display();
		cout << endl;
		mw.display();
		cout << endl;
		break;
	default:
		break;
	}
	cout << "***********************" << endl;
	cout << endl;
}

//sign extend
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

//zero extend
string MIPSSimulator::zeroExtend(string immField)
{
	string imm;
	imm += "0000000000000000" + immField;
	return imm;
}

//decimal to hex
string MIPSSimulator::DToH(int n)
{
	string buffer;
	int remainder;
	while (n != 0)
	{
		remainder = n % 16;
		n /= 16;
		if (remainder < 10)
		{
			buffer += to_string(remainder);
		}
		else
		{
			switch (remainder)
			{
			case 10:
				buffer += 'A';
				break;
			case 11:
				buffer += 'B';
				break;
			case 12:
				buffer += 'C';
				break;
			case 13:
				buffer += 'D';
				break;
			case 14:
				buffer += 'E';
				break;
			case 15:
				buffer += 'F';
				break;
			default:
				break;
			}
		}
	}
	reverse(buffer.begin(),buffer.end());
	return buffer;
}

double MIPSSimulator::utilization(int stage)
{
	double Utlization = 0;
	switch (stage)
	{
	case 0:
		Utlization = (cycleClk - fetchStall) / cycleClk;
		break;
	case 1:
		Utlization = (cycleClk - decodeStall) / cycleClk;
		break;
	case 2:
		Utlization = (cycleClk - executeStall) / cycleClk;
		break;
	case 3:
		Utlization = (cycleClk - memoryStall) / cycleClk;
		break;
	case 4:
		Utlization = (cycleClk - writeBackStall) / cycleClk;
		break;
	default:
		break;
	}
	return Utlization*100;
}
