#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct CCommand
{
	CCommand(const string& line);
	bool IsOn;
	int MinX;
	int MaxX;

	int MinY;
	int MaxY;

	int MinZ;
	int MaxZ;
};

void expectAndConsume(istream& is, char expected) {
	char ch;
	bool res = (bool)(is >> ch);
	assert(res);
	assert(ch == expected);
}

void expectAndConsume(istream& is, const char* str) {
	for (const char* ch = str; *ch != '\0'; ++ch) {
		expectAndConsume(is,*ch);
	}
}

void readInt(istream& is, int& val) {
	bool res = (bool)(is >> val);
	assert(res);

}

CCommand::CCommand(const string& line)
{
	istringstream iss(line);
	string onOff;
	
	bool res = (bool)(iss >> onOff);
	assert(res);
	assert(onOff == "on" || onOff == "off");
	IsOn = onOff == "on";
	expectAndConsume(iss,"x=");
	readInt(iss,MinX);
	expectAndConsume(iss, "..");
	readInt(iss, MaxX);
	expectAndConsume(iss, ",y=");
	readInt(iss, MinY);
	expectAndConsume(iss, "..");
	readInt(iss, MaxY);
	expectAndConsume(iss, ",z=");
	readInt(iss, MinZ);
	expectAndConsume(iss, "..");
	readInt(iss, MaxZ);
}

struct Core
{
	Core()
	{

		for (int x = MIN_X; x <= MAX_X; ++x) {
			for (int y = MIN_Y; y <= MAX_Y; ++y) {
				for (int z = MIN_Z; z <= MAX_Z; ++z) {
					at(x,y,z) = false;
				}
			}
		}
	}

	bool& at(int x, int y, int z) {
		assert(x >= MIN_X && x <= MAX_X);
		assert(y >= MIN_Y && y <= MAX_Y);
		assert(z >= MIN_Z && z <= MAX_Z);
		int nx = x - MIN_X;
		int ny = y - MIN_Y;
		int nz = z - MIN_Z;
		return States[nx][ny][nz];
	}

	void doCommand(const CCommand& command) {
		int minX = max(MIN_X, command.MinX);
		int maxX = min(MAX_X, command.MaxX);
		int minY = max(MIN_Y, command.MinY);
		int maxY = min(MAX_Y, command.MaxY);
		int minZ = max(MIN_Z, command.MinZ);
		int maxZ = min(MAX_Z, command.MaxZ);
		for (int x = minX; x <= maxX; ++x) {
			for (int y = minY; y <= maxY; ++y) {
				for (int z = minZ; z <= maxZ; ++z) {
					at(x, y, z) = command.IsOn;
				}
			}
		}
	}

	int countOn() {
		int count = 0;
		for (int x = MIN_X; x <= MAX_X; ++x) {
			for (int y = MIN_Y; y <= MAX_Y; ++y) {
				for (int z = MIN_Z; z <= MAX_Z; ++z) {
					if (at(x, y, z)) {
						++count;
					}
				}
			}
		}
		return count;
	}

	static const int MIN_X = -50;
	static const int MIN_Y = -50;
	static const int MIN_Z = -50;
	static const int MAX_X = 50;
	static const int MAX_Y = 50;
	static const int MAX_Z = 50;
	array<array<array<bool, MAX_X - MIN_X + 1>, MAX_Y - MIN_Y + 1>, MAX_Z - MIN_Z + 1> States;
	
};

int partA(const vector<CCommand>& commands)
{
	unique_ptr<Core> core{new Core()};
	for (auto& command : commands) {
		core->doCommand(command);
	}
	return core->countOn();
}

struct OffBlock
{
	OffBlock(const CCommand& command)
	{
		assert(!command.IsOn);
		MinX = command.MinX;
		MaxX = command.MaxX;
		MinY = command.MinY;
		MaxY = command.MaxY;
		MinZ = command.MinZ;
		MaxZ = command.MaxZ;
	}

	OffBlock(int minX, int maxX, int minY, int maxY, int minZ, int maxZ)
		: MinX(minX), MaxX(maxX), MinY(minY), MaxY(maxY), MinZ(minZ), MaxZ(maxZ) {}


	int MinX;
	int MaxX;

	int MinY;
	int MaxY;

	int MinZ;
	int MaxZ;
};

struct OnBlock
{
	OnBlock(const CCommand& command)
	{
		assert(command.IsOn);
		MinX = command.MinX;
		MaxX = command.MaxX;
		MinY = command.MinY;
		MaxY = command.MaxY;
		MinZ = command.MinZ;
		MaxZ = command.MaxZ;
	}

	OnBlock(int minX,int maxX, int minY, int maxY, int minZ, int maxZ) 
	: MinX(minX), MaxX(maxX), MinY(minY), MaxY(maxY), MinZ(minZ), MaxZ(maxZ) {}


	int64_t GetVolume() const {
		int64_t retVal = (MaxX - MinX + 1);
		retVal *= (MaxY - MinY + 1);
		retVal *= (MaxZ - MinZ + 1);
		return retVal;
	}

	bool IsValid() const {
		return MaxX >= MinX && MaxY >= MinY && MaxZ >= MinZ;
	}

	void ClampWithin(const OnBlock& parent) {
		MinX = max(MinX, parent.MinX);
		MinY = max(MinY, parent.MinY);
		MinZ = max(MinZ, parent.MinZ);

		MaxX = min(MaxX, parent.MaxX);
		MaxY = min(MaxY, parent.MaxY);
		MaxZ = min(MaxZ, parent.MaxZ);
	}

	vector<OnBlock> Intersect(const OffBlock& offBlock);
	vector<OnBlock> Intersect(const OnBlock& onBlock);

	int MinX;
	int MaxX;

	int MinY;
	int MaxY;

	int MinZ;
	int MaxZ;
};

vector<OnBlock> OnBlock::Intersect(const OffBlock& offBlock)
{
	vector<OnBlock> retVal;
	if (offBlock.MaxX < MinX ||
		offBlock.MinX > MaxX ||
		offBlock.MaxY < MinY ||
		offBlock.MinY > MaxY ||
		offBlock.MaxZ < MinZ ||
		offBlock.MinZ > MaxZ)
	{
		retVal.push_back(*this);
		return retVal;
	}
	OnBlock left(MinX,offBlock.MinX-1,MinY,MaxY,MinZ,MaxZ);
	left.ClampWithin(*this);
	OnBlock right(offBlock.MaxX+1,MaxX,MinY,MaxY,MinZ,MaxZ);
	right.ClampWithin(*this);
	OnBlock top(offBlock.MinX, offBlock.MaxX, offBlock.MinY, offBlock.MaxY,offBlock.MaxZ+1, MaxZ);
	top.ClampWithin(*this);
	OnBlock bottom(offBlock.MinX, offBlock.MaxX, offBlock.MinY, offBlock.MaxY, MinZ, offBlock.MinZ-1);
	bottom.ClampWithin(*this);
	OnBlock front(offBlock.MinX, offBlock.MaxX, MinY, offBlock.MinY-1, MinZ, MaxZ);
	front.ClampWithin(*this);
	OnBlock back(offBlock.MinX, offBlock.MaxX, offBlock.MaxY+1, MaxY, MinZ, MaxZ);
	back.ClampWithin(*this);

	if (left.IsValid()) {
		retVal.push_back(left);
	}
	if (right.IsValid()) {
		retVal.push_back(right);
	}
	if (top.IsValid()) {
		retVal.push_back(top);
	}
	if (bottom.IsValid()) {
		retVal.push_back(bottom);
	}
	if (front.IsValid()) {
		retVal.push_back(front);
	}
	if (back.IsValid()) {
		retVal.push_back(back);
	}
	return retVal;
}

vector<OnBlock> OnBlock::Intersect(const OnBlock& onBlock)
{
	vector<OnBlock> retVal;
	if (onBlock.MaxX < MinX || 
		onBlock.MinX > MaxX || 
		onBlock.MaxY < MinY || 
		onBlock.MinY > MaxY || 
		onBlock.MaxZ < MinZ || 
		onBlock.MinZ > MaxZ)
	{
		retVal.push_back(*this);
		return retVal;
	}
	OffBlock overlap(max(MinX, onBlock.MinX), min(MaxX, onBlock.MaxX), max(MinY, onBlock.MinY), min(MaxY, onBlock.MaxY), max(MinZ, onBlock.MinZ), min(MaxZ, onBlock.MaxZ));
	return Intersect(overlap);
}

int64_t partB(const vector<CCommand>& commands)
{
	vector<OnBlock> onBlocks;
	for (const CCommand& c : commands) {
		if (c.IsOn)
		{
			vector<OnBlock> newOnBlocks;
			OnBlock onBlock(c);
			for (OnBlock& ob : onBlocks) {
				vector<OnBlock> splits = ob.Intersect(onBlock);
				for (OnBlock sob : splits) {
					newOnBlocks.push_back(sob);
				}
			}
			newOnBlocks.push_back(onBlock);
			onBlocks.swap(newOnBlocks);
		}
		else
		{
			vector<OnBlock> newOnBlocks;
			OffBlock offBlock(c);
			for (OnBlock& ob : onBlocks) {
				vector<OnBlock> splits = ob.Intersect(offBlock);
				for (OnBlock sob : splits) {
					newOnBlocks.push_back(sob);
				}
			}
			onBlocks.swap(newOnBlocks);
		}
	}
	int64_t sum = 0;
	for (OnBlock& ob : onBlocks) {
		sum += ob.GetVolume();
	}
	return sum;
}

int main()
{
	vector<CCommand> commands;
    string line;
	while (getline(cin, line)) {
		if(line.empty()) continue;
		commands.push_back(CCommand{line});
	}
	cout << "Part A: " << partA(commands) << '\n';
	cout << "Part B: " << partB(commands) << '\n';
	return 0;
}