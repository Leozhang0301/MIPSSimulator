#include <iostream>
#include <string>
#include <fstream>
#include "include.h"
using namespace std;

string IMEM[MEMSIZE] = {"none"};//instructions memory
int DMEM[MEMSIZE] = {0};//data memory
int numberOfInstruction=0;//number of instructions

void writeInMem(string ipath)
{
	ifstream file;
	file.open(ipath, ios::in);
	if (!file)
	{
		cout << "Error: File does not exist or could not be opened" << endl;
		exit(1);
	}
	string tempString;
	while (getline(file, tempString))//read line by line
	{
		if (numberOfInstruction > MEMSIZE)
		{
			cout << "Error: Instruction memory overflow" << endl;
			exit(1);
		}
		IMEM[numberOfInstruction%MEMSIZE] = tempString;//push into instruction memory
		numberOfInstruction++;
	}
	file.close();
}

int main()
{
	string path = "test.txt";
	writeInMem(path);
	cout << "welcome to MIPS simulator" << endl;
	cout << "please decide which mode do you want to simulate..." << endl;
	cout << "1:instruction by instruction   2:cycle by cycle" << endl;
	cout << "ONLY ACCEPT 1 OR 2!!!" << endl;
	int mode=0;
	int numberToExcute=0;
	cin >> mode;
	while (mode != 1 && mode != 2)
	{
		cout << "please input 1 or 2 ONLY" << endl;
		cin >> mode;
	}
	cout << "How many instructions do you want to simulate..." << endl;
	cin >> numberToExcute;
	MIPSSimulator *simulator = new MIPSSimulator(mode, numberToExcute);
	simulator->process();
	return 0;
}