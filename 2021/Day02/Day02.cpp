// Day02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cstdint>
#include <iostream>
#include <string>

using namespace std;

struct STATEB
{
	int64_t Depth = 0;
	int64_t Horizontal = 0;
	int64_t Aim = 0;
};

struct STATEA
{
	int Depth = 0;
	int Horizontal = 0;
};

int partA(const STATEA& s)
{
	return s.Depth * s.Horizontal;
}

int64_t partB(const STATEB& s)
{
	return s.Depth * s.Horizontal;
}

int main()
{
	STATEA stateA;
	STATEB stateB;
	string cmd;
	int delta;
	while (cin >> cmd >> delta)
	{
		if (cmd == "forward")
		{
			stateA.Horizontal += delta;
			stateB.Horizontal += delta;
			stateB.Depth += delta * stateB.Aim;
		}
		else if (cmd == "up")
		{
			stateA.Depth -= delta;
			stateB.Aim -= delta;
		}
		else
		{
			stateA.Depth += delta;
			stateB.Aim += delta;
		}
	}

	std::cout << "Part A: " << partA(stateA) << '\n';
    std::cout << "Part B: " << partB(stateB) << '\n';
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
