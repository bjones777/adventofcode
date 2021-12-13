// Day11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

using namespace std;

class Grid
{
public:
	Grid(istream& is) {
		Levels.reserve(DIM*DIM);
		for (int r = 0; r < DIM; ++r) {
			for (int c = 0; c < DIM; ++c) {
				char ch;
				cin >> ch;
				int l = ch - '0';
				Levels.push_back(l);
			}
		}
	}

	Grid Step() const;

	int GetTotalNumFlashes() const {
		return NumFlashedTotal;
	}

	bool DidAllFlash() const {
		return NumFlashedThisStep == DIM*DIM;
	}
private:
	bool GetHasFlashed(int r, int c) const {
		return HasFlashed.at(r * DIM + c);
	}

	void SetHasFlashed(int r, int c) {
		HasFlashed.at(r * DIM + c) = true;
	}

	int GetLevel(int r, int c) const {
		return Levels.at(r * DIM + c);
	}

	void SetLevel(int r, int c, int level) {
		Levels.at(r * DIM + c) = level;
	}

	void FlashIfEligible(int r,int c);
	void Flash(int r,int c);

	static const int DIM = 10;
	vector<int> Levels;
	vector<bool> HasFlashed;

	int NumFlashedTotal = 0;
	int NumFlashedThisStep = 0;
};

Grid Grid::Step() const
{
	Grid retVal{*this};
	retVal.HasFlashed.clear();
	retVal.HasFlashed.resize(DIM*DIM, false);
	retVal.NumFlashedThisStep = 0;
	for (int r = 0; r < DIM; ++r) {
		for (int c = 0; c < DIM; ++c) {
			retVal.SetLevel(r, c, GetLevel(r, c) + 1);
		}
	}
	for (int r = 0; r < DIM; ++r) {
		for (int c = 0; c < DIM; ++c) {
			retVal.FlashIfEligible(r,c);
		}
	}
	return retVal;
}

void Grid::FlashIfEligible(int r, int c)
{
	int level = GetLevel(r,c);
	if (level >= 10)
	{
		Flash(r,c);
	}
}

void Grid::Flash(int r, int c)
{
	NumFlashedThisStep += 1;
	NumFlashedTotal += 1;
	SetHasFlashed(r,c);
	SetLevel(r,c,0);
	for (int dr = -1; dr <= 1; ++dr) {
		int nr = r + dr;
		if(nr < 0 || nr >= DIM) continue;
		for (int dc = -1; dc <= 1; ++dc) {
			int nc = c + dc;
			if(nc < 0 || nc >= DIM) continue;
			if(GetHasFlashed(nr,nc)) continue;
			SetLevel(nr, nc, GetLevel(nr, nc) + 1);
			FlashIfEligible(nr, nc);
		}
	}
}

static int partA(const Grid& grid)
{
	Grid newGrid = grid;
	for (int i = 0; i < 100; ++i) {
		newGrid = newGrid.Step();
	}
	return newGrid.GetTotalNumFlashes();
}

static int partB(const Grid& grid)
{
	Grid newGrid = grid;
	int numSteps = 0;
	do 
	{
		newGrid = newGrid.Step();
		++numSteps;
	} while (!newGrid.DidAllFlash());
	return numSteps;
}

int main()
{
    Grid grid{cin};
	cout << "Part A: " << partA(grid) << '\n';
	cout << "Part B: " << partB(grid) << '\n';
	return 0;
}