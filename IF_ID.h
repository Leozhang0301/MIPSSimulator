#pragma once

class IF_ID
{
public:
	IF_ID();
	~IF_ID();
	void setIR(string IR);
	string getIR();
	void setNPC(int NPC);
	int getNPC();
private:
	string IR;//instruction
	int NPC;//program counter
};

