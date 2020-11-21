#pragma once
#ifndef _MAIN
#define _MAIN
#define MEMSIZE 2048
#include <string>
using namespace std;
//global variables
extern string IMEM[MEMSIZE];//instructions memory
extern int DMEM[MEMSIZE];//data memory
extern int numberOfInstruction;//number of instructions
#endif // _MAIN
