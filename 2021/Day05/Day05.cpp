// Day05.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <unordered_map>

using namespace std;

class Point2D
{
public:
	Point2D(int ix,int iy) : x(ix), y(iy) {}
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

class Line2D
{
public:
	Line2D(const Point2D& ip0, const Point2D& ip1)
	: p0{ip0}, p1{ip1}
	{}

	void RenderA(unordered_map<Point2D, int>& um) const
	{
		Render(um, false);
	}
	void RenderB(unordered_map<Point2D, int>& um) const
	{
		Render(um, true);
	}

private:
	void Render(unordered_map<Point2D, int>& um, bool isPartB) const;
	Point2D p0;
	Point2D p1;
};

void Line2D::Render(unordered_map<Point2D, int>& units, bool isPartB) const
{
	if (p0.GetX() - p1.GetX() != 0) {
		if (p1.GetY() == p0.GetY()) {
			int lx = min(p0.GetX(), p1.GetX());
			int ux = max(p0.GetX(), p1.GetX());

			for (int x = lx; x <= ux; ++x) {
				Point2D p{ x, p0.GetY() };
				units[p] += 1;
			}
		}
		else if(isPartB)
		{
			int dx = (p0.GetX() - p1.GetX()) > 0 ? -1 : 1;
			int dy = (p0.GetY() - p1.GetY()) > 0 ? -1 : 1;

			Point2D d{ dx,dy };
			Point2D p = p0;
			while (p != p1) {
				units[p] += 1;
				p += d;
			}
			units[p] += 1;

		}
	}
	else {
		if (p0.GetX() == p1.GetX())
		{
			int ly = min(p0.GetY(), p1.GetY());
			int uy = max(p0.GetY(), p1.GetY());

			for (int y = ly; y <= uy; ++y) {
				Point2D p{ p0.GetX(), y };
				units[p] += 1;
			}
		}
		else if (isPartB) {
			int dx = (p0.GetX() - p1.GetX()) > 0 ? -1 : 1;
			int dy = (p0.GetY() - p1.GetY()) > 0 ? -1 : 1;

			Point2D d{ dx,dy };
			Point2D p = p0;
			while (p != p1) {
				units[p] += 1;
				p += d;
			}
			units[p] += 1;
		}
	}
}


int partA(const vector<Line2D>& lines) {
	unordered_map<Point2D, int> units;
	for (auto& line : lines)
	{
		line.RenderA(units);
	}
	int count = 0;
	for (auto& kvp : units) {
		if (kvp.second > 1) {
			++count;
		}
	}
	return count;
}

int partB(const vector<Line2D>& lines) {
	unordered_map<Point2D, int> units;
	for (auto& line : lines)
	{
		line.RenderB(units);
	}
	int count = 0;
	for (auto& kvp : units) {
		if (kvp.second > 1) {
			++count;
		}
	}
	return count;
}

int main()
{
	vector<Line2D> lines;
	int x1, y1, x2, y2;
	char s1, s2, s3, s4;
	while (cin >> x1 >> s1 >> y1 >> s2 >> s3 >> x2 >> s4 >> y2) {
		assert(s1 == ',');
		assert(s4 == ',');
		assert(s2 == '-');
		assert(s3 == '>');
		Point2D p0{x1,y1}, p1{x2, y2};

		lines.emplace_back(Line2D{p0, p1});
		
	}
	cout << "Part A: " << partA(lines) << '\n';
	cout << "Part B: " << partB(lines) << '\n';
	return 0;
}
