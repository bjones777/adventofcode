#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

vector<int> toVector(const string& s)
{
	istringstream iss(s);
	vector<int> retVal;
	int x;
	char spacer;
	while (iss >> x)
	{
		retVal.emplace_back(x);
		if (iss >> spacer) {
			assert(spacer == ',');
		}
	}
	return retVal;
}

static int pos_diff(const vector<int>& pos, int v) {
	int total = 0;
	for (int p : pos)
	{
		total += abs(v - p);
	}
	return total;
}

static int64_t pos_diff2(const vector<int>& pos, int v) {
	int64_t total = 0;
	for (int p : pos)
	{
		int64_t diff = abs(v - p);
		diff = (diff*(diff + 1)) / 2;
		total += diff;
	}
	return total;
}

static int partA(vector<int> pos)
{
	int minVal = *min_element(pos.begin(), pos.end());
	int maxVal = *max_element(pos.begin(), pos.end());

	int minDiff = numeric_limits<int>::max();
	for (int v = minVal; v <= maxVal; ++v) {
		int diff = pos_diff(pos, v);
		if (diff < minDiff) {
			minDiff = diff;
		}
	}
	return minDiff;
}

static int64_t partB(vector<int> pos)
{
	int minVal = *min_element(pos.begin(), pos.end());
	int maxVal = *max_element(pos.begin(), pos.end());

	int64_t minDiff = numeric_limits<int64_t>::max();
	for (int v = minVal; v <= maxVal; ++v) {
		int64_t diff = pos_diff2(pos, v);
		if (diff < minDiff) {
			minDiff = diff;
		}
	}
	return minDiff;
}

int main(int argc, char* argv[])
{
	string line;
	cin >> line;
	vector<int> pos = toVector(line);
	cout << "Part A: " << partA(pos) << '\n';
	cout << "Part B: " << partB(pos) << '\n';
	return 0;
}
