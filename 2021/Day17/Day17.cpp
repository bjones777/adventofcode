#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

class Point2D
{
public:
	Point2D(int ix, int iy) : x(ix), y(iy) {}
	int GetX() const { return x; }
	int GetY() const { return y; }

	bool operator==(const Point2D& rhs) const {
		return x == rhs.x && y == rhs.y;
	}

	bool operator!=(const Point2D& rhs) const {
		return x != rhs.x || y != rhs.y;
	}

	Point2D& operator+=(const Point2D& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
private:
	int x;
	int y;
};

namespace std {
	template <>
	struct hash<Point2D>
	{
		std::size_t operator()(const Point2D& k) const
		{

			return ((hash<int>()(k.GetX())
				^ (hash<int>()(k.GetY()) << 1) >> 1));
		}
	};
}

struct Box
{
	Box(int minX,int minY, int maxX, int maxY) 
	: extentMin(minX,minY), extentMax(maxX,maxY)
	{}

	bool IsInside(const Point2D& point) const
	{
		if(point.GetX() < extentMin.GetX()) return false;
		if(point.GetX() > extentMax.GetX()) return false;
		if(point.GetY() < extentMin.GetY()) return false;
		if(point.GetY() > extentMax.GetY()) return false;
		return true;
	}

	Point2D extentMin;
	Point2D extentMax;
};

struct Projectile
{
	Projectile(int dx, int dy) : Location{0,0}, Velocity{dx,dy} {
	}

	void Step()
	{
		Location += Velocity;
		if (Velocity.GetX() < 0) {
			Velocity += Point2D{+1, -1};
		}
		else if (Velocity.GetX() > 0) {
			Velocity += Point2D{ - 1,  -1 };
		}
		else {
			Velocity += Point2D{ 0, - 1 };
		}
	}

	bool IsPast(const Box& box) {
		if (Velocity.GetX() >= 0 && Location.GetX() > box.extentMax.GetX()) {
			return true;
		}
		if (Velocity.GetX() <= 0 && Location.GetX() < box.extentMin.GetX()) {
			return true;
		}
		if (Velocity.GetY() <= 0 && Location.GetY() < box.extentMin.GetY()) {
			return true;
		}
		return false;
	}

	Point2D GetLocation() const {
		return Location;
	}

	Point2D Location;
	Point2D Velocity;
};

int Simulate(int dx, int dy, const Box& target) {
	Projectile p{dx,dy};
	int maxHeight = p.Location.GetY();
	do {
		if (target.IsInside(p.GetLocation())) {
			return maxHeight;
		}
		p.Step();
		maxHeight = max(maxHeight, p.GetLocation().GetY());
	} while(!p.IsPast(target));
	return numeric_limits<int>::min();
}

static int partA(const Box& target) {
	int maxHeight = 0;
	for (int x = 0; x <= 1000; ++x) {
		for(int y = -500;y <= 500;++y) {
			int height = Simulate(x,y, target);
			if(height == numeric_limits<int>::min()) continue;
			if (height > maxHeight)
			{
				maxHeight = height;
			}
		}
	}
	return maxHeight;
}

static int partB(const Box& target) {
	int count = 0;
	for (int x = 0; x <= 1000; ++x) {
		for (int y = -500; y <= 500; ++y) {
			int height = Simulate(x, y, target);
			if (height != numeric_limits<int>::min())
			{
				++count;
			}
		}
	}
	return count;
}


int main()
{
	//target area: x=70..125, y=-159..-121
	Box target{70,-159, 125, -121};
	int sol = partA(target);
    cout << "Part A: " << sol << '\n';

	sol = partB(target);
	cout << "Part B: " << sol << '\n';
	return 0;
}