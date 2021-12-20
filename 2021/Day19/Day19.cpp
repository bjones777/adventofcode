// Day19.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class Point3D
{
public:
	Point3D(int ix, int iy, int iz) : x(ix), y(iy), z(iz) {}
	int GetX() const { return x; }
	int GetY() const { return y; }
	int GetZ() const { return z; }

	bool operator==(const Point3D& rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	bool operator!=(const Point3D& rhs) const {
		return x != rhs.x || y != rhs.y || z != rhs.z;
	}

	Point3D& operator+=(const Point3D& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Point3D& operator-=(const Point3D& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Point3D operator-(const Point3D& rhs) {
		Point3D t{*this};
		return (t -= rhs);
	}

	Point3D rotateX() const {
		return Point3D(x,z,-y);
	}

	Point3D rotateY() const {
		return Point3D(-z,y,x); 
	}

	Point3D rotateZ() const {
		return Point3D(-y, x, z);
	}

	Point3D Swizzle(const int swizzle[3]) {
		return Point3D(Swizzle(swizzle[0]),
					   Swizzle(swizzle[1]),
					   Swizzle(swizzle[2]));
	}

	int GetDistance() const {
		return abs(x) + abs(y) + abs(z);
	}
private:
	int Swizzle(int index) {
		switch (index) {
			case -1:
				return -x;
			case 1:
				return x;
			case -2:
				return -y;
			case 2:
				return y;
			case -3:
				return -z;
			case 3:
				return z;
			default:
				throw runtime_error("bad swizzle");
		}
	}
	
	int x;
	int y;
	int z;
};


namespace std {

	template <>
	struct hash<Point3D>
	{
		std::size_t operator()(const Point3D& k) const
		{

			return ((hash<int>()(k.GetX())
				^ (hash<int>()(k.GetY()) << 1) >> 1)
				^ (hash<int>()(k.GetZ()) << 2) >> 2);
		}
	};
}

ostream& operator<<(ostream& os, const Point3D& p) {
	return (os << "(" << p.GetX() << "," << p.GetY() << "," << p.GetZ() << ")");
}


class World
{
public:
	bool Project(const Point3D& point, int id);
	void Unproject(const Point3D& point, int id);

	Point3D PickRandomPoint();

	int GetNumSignals() const {
		int count = 0;
		for (auto& kvp : Signals) {
			if (!kvp.second.empty()) {
				++count;
			}
		}
		return count;
	}
private:
	unordered_map<Point3D, unordered_set<int>> Signals;
};

Point3D World::PickRandomPoint()
{
	int index = rand() % Signals.size();
	auto iter = Signals.begin();
	advance(iter, index);
	return iter->first;
}

bool World::Project(const Point3D& point, int id)
{
	auto& set = Signals[point];
	auto iter = set.find(id);
	if (iter == set.end()) {
		bool occupy = set.size() != 0;
		set.insert(id);
		return occupy;
	}
	else {
		assert(false);
	}
	throw runtime_error("dupe project");
}

void World::Unproject(const Point3D& point, int id)
{
	auto& set = Signals[point];
	auto iter = set.find(id);
	if (iter != set.end()) {
		set.erase(iter);
		if (set.size() == 0) {
			auto siter = Signals.find(point);
			Signals.erase(siter);
		}
	}
	else {
		assert(false);
	}
}

class Scanner
{
public:
	Scanner(int id) : Id(id) {}

	void AddPoint(const Point3D& point) {
		Points.push_back(point);
	}

	int Project(World& world) {
		int count = 0;
		for (Point3D p : Points) {
			Point3D np = Transform(p);
			if (world.Project(np, Id)) {
				++count;
			}
		}
		return count;
	}

	void Unproject(World& world) {
		for (Point3D p : Points) {
			Point3D np = Transform(p);
			world.Unproject(np, Id);
		}
	}

	int GetNumPoints() const {
		return (int)Points.size();
	}

	void Reorient() {
		Swizzle = rand() % 24;
	}

	void SetOriginBasedOnRandomPoint(const Point3D& p);

	Point3D GetOrigin() const {
		return Origin;
	}
private:
	Point3D Transform(const Point3D& p)
	{
		static const int Configs[24][3] = {
			{3,1,2},
			{-3,-1,2},
			{2,-3,-1},
			{1,2,3},
			{2,3,1},
			{-1,2,-3},
			{-1,-3,-2},
			{1,3,-2},
			{-3,-2,-1},
			{-2,-1,-3},
			{-2,1,3},
			{3,-2,1},
			{3,-1,-2},
			{-3,1,-2},
			{-2,3,-1},
			{1,-2,-3},
			{-1,-2,3},
			{-2,-3,1},
			{1,-3,2},
			{-1,3,2},
			{3,2,-1},
			{2,1,-3},
			{2,-1,3},
			{-3,2,1},		
		};
		Point3D np = p;
		np = np.Swizzle(Configs[Swizzle]);
		np += Origin;
		return np;
	}

	int Id;
	vector<Point3D> Points;

	Point3D Origin = Point3D(0,0,0);
	int Swizzle = 0;
};

void Scanner::SetOriginBasedOnRandomPoint(const Point3D& p)
{
	Point3D rpoint = Points.at(rand() % Points.size());
	Point3D np = Transform(rpoint);
	Point3D diff = np-p;
	Origin -= diff;
	assert(p == Transform(rpoint));
}

Point3D doTurns(const Point3D& p, int x, int y, int z) {
	Point3D np = p;
	while (x-- > 0) {
		np = np.rotateX();
	}
	while (y-- > 0) {
		np = np.rotateY();
	}
	while (z-- > 0) {
		np = np.rotateZ();
	}
	return np;
}

void testA()
{
	unordered_set<Point3D> points;
	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {
			for (int z = 0; z < 4; ++z) {
				points.insert(doTurns(Point3D{1,2,3},x,y,z));
			}
		}
	}
	for (auto& p : points) {
		cout << p << "\n";
	}
}

bool IsScannerStart(const string& line) {
	if (line.size() >= 3 && line.substr(0,3) == "---") {
		return true;
	}
	return false;
}

int GetScannerId(const string& line) {
	istringstream iss(line);
	int id;
	string stuff;
	bool res = (bool)(iss >> stuff);
	assert(res);
	assert(stuff == "---");
	res = (bool)(iss >> stuff);
	assert(res);
	assert(stuff == "scanner");
	res = (bool)(iss >> id);
	assert(res);
	return id;
}

Point3D ReadPoint(const string& line) {
	istringstream iss(line);
	char s1, s2;
	int x, y, z;
	bool res = (bool)(iss >> x >> s1 >> y >> s2 >> z);
	assert(res);
	assert(s1 == ',');
	assert(s2 == ',');
	return Point3D(x,y,z);
}

static int partA(vector<Scanner>& inScanners)
{
	World world;
	vector<Scanner*> scannersToPlace;
	for (auto& scanner : inScanners) {
		scannersToPlace.push_back(&scanner);
	}
	Scanner* first = scannersToPlace.back();
	int res = first->Project(world);
	assert(res == 0);
	scannersToPlace.pop_back();
	while (!scannersToPlace.empty()) {
		int si = rand() % scannersToPlace.size();
		Scanner* scanner = scannersToPlace.at(si);
		scanner->Reorient();
		Point3D rpoint = world.PickRandomPoint();
		scanner->SetOriginBasedOnRandomPoint(rpoint);
		int count = scanner->Project(world);
		assert(count >= 1);
		if (count <= 11) {
			scanner->Unproject(world);
		}
		else {
			if (si != scannersToPlace.size()-1) {
				swap(scannersToPlace.back(),scannersToPlace[si]);
			}
			scannersToPlace.pop_back();
		}
	}
	return world.GetNumSignals();
}

static int partB(vector<Scanner>& inScanners)
{
	int maxDistance = 0;
	for (int i = 0; i < inScanners.size()-1; ++i) {
		for (int j = i + 1; j < inScanners.size(); ++j) {
			int dist = (inScanners[i].GetOrigin() - inScanners[j].GetOrigin()).GetDistance();
			maxDistance = max(dist,maxDistance);
		}
	}
	return maxDistance;
}

int main()
{
	//srand(0xDEADBEEF);
	vector<Scanner> scanners;
	string line;
	while (getline(cin, line)) {
		if(line.empty()) continue;
		if (IsScannerStart(line)) {
			scanners.emplace_back(Scanner{GetScannerId(line)});
		}
		else {
			scanners.back().AddPoint(ReadPoint(line));
		}
	}
	int res = partA(scanners);
    cout << "Part A: " <<  res << '\n';
	res = partB(scanners);
	cout << "Part B: " << res << '\n';
	return 0;
}

