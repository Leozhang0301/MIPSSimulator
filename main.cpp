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
	string path = "binary.txt";
	//before write in memory
	for (int i = 0; i < numberOfInstruction; i++)
	{
		cout << IMEM[i] << endl;
	}
	writeInMem(path);
	//after write in memory
	for (int i = 0; i < numberOfInstruction; i++)
	{
		cout << IMEM[i] <<endl;
	}
	cout << numberOfInstruction;
	return 0;
}