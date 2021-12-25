
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class EDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

static int GetNumDirections() {
	return ((int)EDirection::RIGHT) + 1;
}

static EDirection GetDirectionAtIndex(int index) {
	assert(index >= 0 && index < GetNumDirections());
	return static_cast<EDirection>(index);
}

struct RowCol
{
	RowCol() : Row(0), Column(0) {}
	RowCol(int r, int c) : Row(r), Column(c) {}
	int Row;
	int Column;

	RowCol Moved(EDirection dir) const;

	bool operator==(const RowCol& rhs) const {
		return Row == rhs.Row && Column == rhs.Column;
	}
};

RowCol RowCol::Moved(EDirection dir) const
{
	switch (dir)
	{
	case EDirection::UP:
		return RowCol(Row - 1, Column);
	case EDirection::DOWN:
		return RowCol(Row + 1, Column);
	case EDirection::LEFT:
		return RowCol(Row, Column - 1);
	}
	assert(dir == EDirection::RIGHT);
	return RowCol(Row, Column + 1);
}


enum class ESpaceType
{
	EMPTY,
	EAST,
	SOUTH,
};

class Grid
{
public:
	void AddLine(const string& line)
	{
		if (Width == 0)
		{
			Width = (int)line.size();
		}
		else
		{
			assert(Width == (int)line.size());
		}
		for (char ch : line)
		{
			switch (ch)
			{
				case '.':
					Spaces.push_back(ESpaceType::EMPTY);
					break;
				case '>':
					Spaces.push_back(ESpaceType::EAST);
					break;
				case 'v':
					Spaces.push_back(ESpaceType::SOUTH);
					break;
				default:
					assert(false);
			}
		}
		++Height;
	}

	void Print() const
	{
		for (int r = 0; r < Height; ++r) {
			for (int c = 0; c < Width; ++c) {
				switch (Get(r, c))
				{
					case ESpaceType::EAST:
						cout << '>';
						break;
					case ESpaceType::SOUTH:
						cout << 'v';
						break;
					case ESpaceType::EMPTY:
						cout << '.';
						break;
				}
			}
			cout << '\n';
		}
	}

	ESpaceType Get(int r, int c) const {
		return Spaces.at(r * Width + c);
	}

	void Set(int r, int c, ESpaceType spaceType) {
		Spaces.at(r * Width + c) = spaceType;
	}
	
	int GetIndex(int r, int c) const {
		return r * Width + c;
	}

	bool Step()
	{
		bool retVal = false;
		vector<ESpaceType> NewSpaces(Width*Height, ESpaceType::EMPTY);

		// do east
		for (int r = 0; r < Height; ++r) {
			for (int c = 0; c < Width; ++c) {
				if (Get(r, c) == ESpaceType::EAST && Get(r, (c+1) % Width) == ESpaceType::EMPTY) {
					retVal = true;			
					NewSpaces.at(GetIndex(r, (c + 1) % Width)) = ESpaceType::EAST;
				}
				else if (Get(r, c) == ESpaceType::EAST)
				{
					NewSpaces.at(GetIndex(r,c)) = ESpaceType::EAST;
				}
			}
		}

		for (int r = 0; r < Height; ++r) {
			for (int c = 0; c < Width; ++c) {
				if (Get(r, c) == ESpaceType::EAST && NewSpaces.at(GetIndex(r, c)) == ESpaceType::EMPTY)
				{
					Set(r,c, ESpaceType::EMPTY);
				}
				else if (Get(r, c) == ESpaceType::EMPTY && NewSpaces.at(GetIndex(r, c)) == ESpaceType::EAST)
				{ 
					Set(r, c, ESpaceType::EAST);
				}
				NewSpaces.at(GetIndex(r, c)) = ESpaceType::EMPTY;
			}
		}

		// do south
		for (int r = 0; r < Height; ++r) {
			for (int c = 0; c < Width; ++c) {
				if (Get(r, c) == ESpaceType::SOUTH && Get((r+1) % Height, c) == ESpaceType::EMPTY ) {
					retVal = true;
					NewSpaces.at(GetIndex((r + 1) % Height, c)) = ESpaceType::SOUTH;
				}
				else if (Get(r, c) == ESpaceType::SOUTH)
				{
					NewSpaces.at(GetIndex(r,c)) = ESpaceType::SOUTH;
				}
			}
		}

		for (int r = 0; r < Height; ++r) {
			for (int c = 0; c < Width; ++c) {
				if (Get(r, c) == ESpaceType::SOUTH && NewSpaces.at(GetIndex(r, c)) == ESpaceType::EMPTY)
				{
					Set(r, c, ESpaceType::EMPTY);
				}
				else if (Get(r, c) == ESpaceType::EMPTY && NewSpaces.at(GetIndex(r, c)) == ESpaceType::SOUTH)
				{
					Set(r, c, ESpaceType::SOUTH);
				}
				NewSpaces.at(GetIndex(r, c)) = ESpaceType::EMPTY;
			}
		}
		
		return retVal;
	}


private:
	vector<ESpaceType> Spaces;
	int Width = 0;
	int Height = 0;

};

int main()
{
	Grid grid;
	string line;
	while (getline(cin, line)) {
		if (line.empty()) continue;
		grid.AddLine(line);
	}
	for (int step = 1;;++step)
	{
		if (!grid.Step())
		{
			cout << "Part A: " << step << '\n';
			return 0;
		}
	}
    return 0;
}

