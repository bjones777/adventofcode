#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class VisitedMap
{
public:
	VisitedMap(int inWidth, int inHeight)
	: Width(inWidth), Height(inHeight)
	{
		IsVisited.resize(Width * Height);
	}

	void Set(int r, int c)
	{
		IsVisited.at(r * Width + c) = true;
	}

	bool HasVisited(int r, int c)
	{
		return IsVisited.at(r * Width + c);
	}

private:
	int Width;
	int Height;
	vector<bool> IsVisited;
};

class HeightMap
{
public:
	HeightMap(istream& is);

	int GetWidth() const { return Width; }
	int GetHeight() const { return (int)Heights.size()/Width; }

	int GetCell(int row,int col) const { return Heights.at(row * Width + col); }
private:
	vector<int> Heights;
	int Width;
};

HeightMap::HeightMap(istream& is)
{
	string line;
	is >> line;
	Width = (int)line.size();
	do 
	{
		assert(line.size() == Width);
		for (int i = 0; i < (int)line.size(); ++i) {
			int digit = line[i] - '0';
			assert(digit >= 0 && digit <= 9);
			Heights.push_back(digit);
		}
	} while (is >> line);
}

static bool is_low_point(const HeightMap& heightMap, int row, int col)
{
	int val = heightMap.GetCell(row,col);
	for (int dr = -1; dr <= 1; dr += 2) {
		int nr = row + dr;
		if (nr >= 0 && nr < heightMap.GetHeight())
		{
			if (val >= heightMap.GetCell(nr, col)) {
				return false;
			}
		}
	}
	for (int dc = -1; dc <= 1; dc += 2) {
		int nc = col + dc;
		if (nc >= 0 && nc < heightMap.GetWidth())
		{
			if (val >= heightMap.GetCell(row, nc)) {
				return false;
			}
		}
	}
	return true;
}

static int partA(const HeightMap& heightMap)
{
	int sum = 0;
	for (int r = 0; r < heightMap.GetHeight(); ++r) {
		for (int c = 0; c < heightMap.GetWidth(); ++c) {
			if (is_low_point(heightMap, r, c)) {
				sum += heightMap.GetCell(r,c) + 1;
			}
		}
	}
	return sum;
}

struct Basin
{
	Basin(int r, int c) : Row(r), Col(c) {
		Size = 0;
	}

	bool operator<(const Basin& rhs) const {
		return Size < rhs.Size;
	}
	int Row;
	int Col;
	int Size;
};

static void add_to_basin(Basin& b,const HeightMap& heightMap, int r, int c, VisitedMap& vm)
{
	if(r < 0 || r >= heightMap.GetHeight()) return;
	if (c < 0 || c >= heightMap.GetWidth()) return;
	if(vm.HasVisited(r,c)) return;
	int v = heightMap.GetCell(r,c);
	if(v == 9) return;
	b.Size += 1;
	vm.Set(r,c);
	for (int dr = -1; dr <= 1; dr += 2) {
		int nr = r + dr;
		add_to_basin(b, heightMap, nr, c, vm);
	}
	for (int dc = -1; dc <= 1; dc += 2) {
		int nc = c + dc;
		add_to_basin(b, heightMap, r, nc, vm);
	}
}

static Basin find_basin(const HeightMap& heightMap, int r, int c)
{
	VisitedMap vistedMap(heightMap.GetWidth(), heightMap.GetHeight());
	Basin basin{r,c};
	add_to_basin(basin, heightMap, r, c, vistedMap);
	return basin;
}

static int partB(const HeightMap& heightMap)
{
	vector<Basin> basins;
	for (int r = 0; r < heightMap.GetHeight(); ++r) {
		for (int c = 0; c < heightMap.GetWidth(); ++c) {
			if (is_low_point(heightMap, r, c)) {
				basins.push_back(find_basin(heightMap, r, c));
			}
		}
	}
	sort(basins.begin(), basins.end());
	int product = 1;
	int count = 0;
	for (auto ri = basins.rbegin(); ri != basins.rend() && count < 3; ++count, ++ri) {
		product *= ri->Size;
	}
	return product;
}

int main()
{
    HeightMap heightMap(cin);
	cout << "Part A: " << partA(heightMap) << '\n';
	cout << "Part B: " << partB(heightMap) << '\n';
	return 0;
}
