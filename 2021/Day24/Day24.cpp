// Day24.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <string>
#include <vector>


using namespace std;

/*
inp a - Read an input value and write it to variable a.
add a b - Add the value of a to the value of b, then store the result in variable a.
mul a b - Multiply the value of a by the value of b, then store the result in variable a.
div a b - Divide the value of a by the value of b, truncate the result to an integer, then store the result in variable a. (Here, "truncate" means to round the value toward zero.)
mod a b - Divide the value of a by the value of b, then store the remainder in variable a. (This is also called the modulo operation.)
eql a b
*/

enum class EInstructionType
{
	INP,
	ADD,
	MUL,
	DIV,
	MOD,
	EQL,
};

enum class EOperandType
{
	REGISTER,
	IMMEDIATE,
};

enum class ERegisters
{
	W,
	X,
	Y,
	Z,
};

static const int NUM_REGISTERS = ((int)ERegisters::Z)+1;

class COperand
{
public:
	EOperandType OperandType;
	ERegisters Register;
	int64_t Immediate;
};


class CInstruction
{
	EInstructionType InstructionType;
	array<COperand, 2> Operands;
	int NumOperands = 0;

public:
	CInstruction(const string& Line);
	
	EInstructionType GetInstructionType() const { return InstructionType; }

	const COperand& GetOperandAtIndex(int index) const { 
		assert(index >= 0 && index < NumOperands); 
		return Operands.at(index);
	}

private:
	void ReadRegister(istream& is);
	void ReadRegisterOrImmediate(istream& is);
};

CInstruction::CInstruction(const string& Line)
{
	istringstream iss(Line);
	string instr;
	bool res = (bool)(iss >> instr);
	assert(res);
	if (instr == "inp")
	{
		InstructionType = EInstructionType::INP;
		ReadRegister(iss);
	}
	else if (instr == "add")
	{
		InstructionType = EInstructionType::ADD;
		ReadRegister(iss);
		ReadRegisterOrImmediate(iss);
	}
	else if (instr == "mul")
	{
		InstructionType = EInstructionType::MUL;
		ReadRegister(iss);
		ReadRegisterOrImmediate(iss);
	}
	else if (instr == "div")
	{
		InstructionType = EInstructionType::DIV;
		ReadRegister(iss);
		ReadRegisterOrImmediate(iss);
	}
	else if (instr == "mod")
	{
		InstructionType = EInstructionType::MOD;
		ReadRegister(iss);
		ReadRegisterOrImmediate(iss);
	}
	else
	{
		assert(instr == "eql");
		InstructionType = EInstructionType::EQL;
		ReadRegister(iss);
		ReadRegisterOrImmediate(iss);
	}
}

void CInstruction::ReadRegister(istream& is)
{
	ReadRegisterOrImmediate(is);
	assert(Operands[NumOperands-1].OperandType == EOperandType::REGISTER);
}

void CInstruction::ReadRegisterOrImmediate(istream& is)
{
	assert(NumOperands < Operands.size());
	char ch;
	bool res = (bool)(is >> ch);
	assert(res);
	if ((ch >= '0' && ch <= '9') || ch == '-')
	{
		is.putback(ch);
		int64_t value;
		res = (bool)(is >> value);
		assert(res);
		COperand& newOperand = Operands[NumOperands++];
		newOperand.OperandType = EOperandType::IMMEDIATE;
		newOperand.Immediate = value;
	}
	else
	{
		COperand& newOperand = Operands[NumOperands++];
		newOperand.OperandType = EOperandType::REGISTER;
		assert(ch >= 'w' && ch <= 'z');
		newOperand.Register = (ERegisters)(ch - 'w');
	}
}

class CAlu
{
	array<int64_t, NUM_REGISTERS> Registers;
	string Input;
	int InputIndex = 0;
public:
	CAlu(const string& in) : Input(in)
	{
		for (auto& r : Registers)
		{
			r = 0;
		}
	}

	int64_t GetRegisterValue(ERegisters reg) const
	{
		return Registers.at(GetRegisterIndex(reg));
	}

	void Execute(const CInstruction& Instruction);
	int64_t PeekInput() const {
		assert(InputIndex < Input.size());
		return Input.at(InputIndex) - '0';
	}
private:
	int64_t ReadOperand(const COperand& operand) const;
	void WriteOperand(const COperand& operand, int64_t val);
	int GetRegisterIndex(ERegisters reg) const {
		return (int)reg;
	}
	int64_t ReadInput();
};

int64_t CAlu::ReadInput()
{
	assert(InputIndex < Input.size());
	return Input.at(InputIndex++) - '0';
}

void CAlu::Execute(const CInstruction& Instruction)
{
	switch (Instruction.GetInstructionType())
	{
		case EInstructionType::INP:
			WriteOperand(Instruction.GetOperandAtIndex(0), ReadInput());
			return;
		case EInstructionType::ADD:
			{
				int64_t val = ReadOperand(Instruction.GetOperandAtIndex(0));
				val += ReadOperand(Instruction.GetOperandAtIndex(1));
				WriteOperand(Instruction.GetOperandAtIndex(0),val);
			}
			return;
		case EInstructionType::MUL:
			{
				int64_t val = ReadOperand(Instruction.GetOperandAtIndex(0));
				val *= ReadOperand(Instruction.GetOperandAtIndex(1));
				WriteOperand(Instruction.GetOperandAtIndex(0), val);
			}
			return;
		case EInstructionType::DIV:
			{
				int64_t val = ReadOperand(Instruction.GetOperandAtIndex(0));
				val /= ReadOperand(Instruction.GetOperandAtIndex(1));
				WriteOperand(Instruction.GetOperandAtIndex(0), val);
			}
			return;
		case EInstructionType::MOD:
			{
				int64_t val = ReadOperand(Instruction.GetOperandAtIndex(0));
				val %= ReadOperand(Instruction.GetOperandAtIndex(1));
				WriteOperand(Instruction.GetOperandAtIndex(0), val);
			}
			return;
		case EInstructionType::EQL:
			{
				int64_t val = ReadOperand(Instruction.GetOperandAtIndex(0));
				val = (val == ReadOperand(Instruction.GetOperandAtIndex(1))) ? 1 : 0;
				WriteOperand(Instruction.GetOperandAtIndex(0), val);
			}
			return;
	}
	assert(false && "Unhandled instruction");
}

int64_t CAlu::ReadOperand(const COperand& operand) const
{
	switch (operand.OperandType)
	{
		case EOperandType::IMMEDIATE:
			return operand.Immediate;
	}
	assert(operand.OperandType == EOperandType::REGISTER);
	return Registers.at(GetRegisterIndex(operand.Register));
}

void CAlu::WriteOperand(const COperand& operand, int64_t val)
{
	assert(operand.OperandType == EOperandType::REGISTER);
	Registers.at(GetRegisterIndex(operand.Register)) = val;
}

static int64_t RunMachine(const vector<CInstruction>& Instructions, const string& input)
{
	CAlu alu(input);
	for (const CInstruction& i : Instructions)
	{
		alu.Execute(i);
	}
	return alu.GetRegisterValue(ERegisters::Z);
}

static int64_t RunMachine2(const vector<CInstruction>& Instructions, const string& input)
{
	int numChars = (int)input.size();
	CAlu alu(input);
	for (const CInstruction& i : Instructions)
	{
		if (i.GetInstructionType() == EInstructionType::INP)
		{
			if(numChars == 0) {
				return alu.GetRegisterValue(ERegisters::Z);
			}
			else {
				--numChars;
			}
		}
		alu.Execute(i);
	}
	return alu.GetRegisterValue(ERegisters::Z);
}
const string prefix = "949";
const string suffix = "199";


bool recursiveRunMachine(const vector<CInstruction>& Instructions,int depth,string& buildString)
{
	if (depth == 0)
	{
		string stringToRun = prefix + buildString + suffix;
		if (RunMachine(Instructions, stringToRun) == 0)
		{
			buildString = stringToRun;
			return true;
		}
		return false;
	}
	for (char ch = '9'; ch >= '1'; --ch)
	{
		buildString += ch;
		if (recursiveRunMachine(Instructions, depth - 1, buildString))
		{
			return true;
		}
		else
		{
			buildString.pop_back();
		}
	}
	return false;
}



static string partA(const vector<CInstruction>& Instructions)
{
	//32981141;
	string buildString;
	if (recursiveRunMachine(Instructions, 8, buildString))
	{
		return buildString;
	}
	return "0";
}

const string prefix2 = "119";
const string suffix2 = "161";


bool recursiveRunMachine2(const vector<CInstruction>& Instructions, int depth, string& buildString)
{
	if (depth == 0)
	{
		string stringToRun = prefix2 + buildString + suffix2;
		if (RunMachine(Instructions, stringToRun) == 0)
		{
			buildString = stringToRun;
			return true;
		}
		return false;
	}
	for (char ch = '1'; ch <= '9'; ++ch)
	{
		buildString += ch;
		if (recursiveRunMachine2(Instructions, depth - 1, buildString))
		{
			return true;
		}
		else
		{
			buildString.pop_back();
		}
	}
	return false;
}


static string partB(const vector<CInstruction>& Instructions)
{
	//32981141;
	string buildString;
	if (recursiveRunMachine2(Instructions, 8, buildString))
	{
		return buildString;
	}
	return "0";
}

static string int64_to_string(int64_t i)
{
	assert(i >= 0);
	string digits;
	while (i > 0)
	{
		int digit = i % 10;
		i /= 10;
		digits.push_back(digit + '0');
	}
	reverse(digits.begin(),digits.end());
	return digits;
}

static string partA2(const vector<CInstruction>& Instructions)
{
	int64_t val = 49951881352111;
	for (int i = -25; i <= 25; ++i) {
		string input;
		for (int j = 0; j < 14; ++j)
		{
			input = int64_to_string(val + i);
		}
		int64_t val = RunMachine(Instructions, input);
		cout << "Input: " << input << " is " << (val == 0 ? "VALID" : "INVALID") << " (" << val << ")" << '\n';
	}
	return "0";
}

struct Entry
{
	Entry(const string& input, int64_t z) : Input(input), Z(z) {}
	string Input;
	int64_t Z;

	bool operator>(const Entry& e) const {
		return Z > e.Z;
	}
};

static string partA3(const vector<CInstruction>& Instructions)
{
	array<int64_t,15> minValues;
	for(auto& minValue : minValues)
	{
		minValue = numeric_limits<int64_t>::max();
	}
	int length = 0;
	priority_queue<Entry, vector<Entry>, greater<Entry>> queue;
	queue.push(Entry("",0));
	while (!queue.empty()) {
		Entry e = queue.top();
		queue.pop();
		if (e.Input.size() == 14 && e.Z == 0)
		{
			return e.Input;
		}
		if (minValues.at(e.Input.size()) > e.Z)
		{
			minValues.at(e.Input.size()) = e.Z;
			cout << "Input " << e.Input << " Z= " << e.Z << '\n';
		}
		if (e.Input.size() == 14) continue;
		for (char ch = '9'; ch >= '1'; --ch) {
			string newInput = e.Input;
			newInput += ch;
			Entry ne{newInput, newInput.size() >= 6 ? RunMachine2(Instructions, newInput) : 0 };
			queue.push(ne);
		}
		
	}
	return "";
}

string printString(int64_t v)
{
	string s;
	while (v > 0)
	{
		s += (char)((v % 26) + 'A');
		v /= 26;
	}
	return s;
}

static void partA4(const vector<CInstruction>& Instructions)
{

	string input = "11932981141161";
	input = "94932981141199";

	CAlu alu(input);
	for (const CInstruction& i : Instructions)
	{
		if (i.GetInstructionType() == EInstructionType::INP)
		{
			cout << "Input: " << (char)(alu.PeekInput()+'0') << " Z= " << alu.GetRegisterValue(ERegisters::Z) << " (" << printString(alu.GetRegisterValue(ERegisters::Z)) << ") " << '\n';
		}
		alu.Execute(i);
	}
	if (alu.GetRegisterValue(ERegisters::Z) == 0)
	{
		cout << "VALID\n";
	}
	else
	{
		cout << "INVALID\n";
	}
}



int main()
{
	srand(0xDEADBEEF);
	vector<CInstruction> Instructions;
	string Line;
	while (getline(cin, Line)) {
		if (Line.empty()) continue;
		Instructions.push_back(CInstruction{ Line });
	}
	partA4(Instructions);
	//cout << "Part A:" << partA(Instructions) << "\n";
	cout << "Part B:" << partB(Instructions) << "\n";
	return 0;
}

