// Day20.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_set>

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

	Point2D operator+(const Point2D& rhs) const {
		Point2D t{*this};
		return (t += rhs);
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

class ImageEnhancer;
class Image
{
public:
	Image() {}
	bool IsLit(const Point2D& p) const {
		if (IsAbyssSet) {
			if(p.GetX() < AbyssMinX || p.GetX() > AbyssMaxX) return IsAbyssLit;
			if (p.GetY() < AbyssMinY || p.GetY() > AbyssMaxY) return IsAbyssLit;
		}
		auto iter = Dots.find(p);
		return iter != Dots.end();
	}

	void Lit(const Point2D& p) {
		Dots.insert(p);
	}

	int GetAreaAround(const Point2D& p) const {
		int val = 0;
		for (int dy = -1; dy <= 1; ++dy) {
			for (int dx = -1; dx <= 1; ++dx) {
				val = val << 1;
				if (IsLit(p + Point2D(dx, dy))) {
					val |= 1;
				}
			}
		}
		return val;
	}

	void Print() const;

	Image Enhance(const ImageEnhancer& enhancer);

	int GetNumLit() const { return (int)Dots.size(); }

	void IdentifyAbyss();

	bool IsRowSameValue(int y, int minX, int maxX)
	{
		bool val = IsLit(Point2D(minX,y));
		for (int x = minX; x <= maxX; ++x) {
			if (val != IsLit(Point2D(x, y))) {
				return false;
			}
		}
		return true;
	}

	bool IsColSameValue(int x, int minY, int maxY)
	{
		bool val = IsLit(Point2D(x, minY));
		for (int y = minY; y <= maxY; ++y) {
			if (val != IsLit(Point2D(x, y))) {
				return false;
			}
		}
		return true;
	}
private:
	unordered_set<Point2D> Dots;
	bool IsAbyssSet = false;
	int AbyssMinX = 0;
	int AbyssMaxX = 0;
	int AbyssMinY = 0;
	int AbyssMaxY = 0;
	bool IsAbyssLit = false;
};

void Image::IdentifyAbyss() {
	int minX = numeric_limits<int>::max();
	int maxX = numeric_limits<int>::min();

	int minY = numeric_limits<int>::max();
	int maxY = numeric_limits<int>::min();

	for (auto& p : Dots) {
		minX = min(p.GetX(), minX);
		maxX = max(p.GetX(), maxX);

		minY = min(p.GetY(), minY);
		maxY = max(p.GetY(), maxY);
	}

	AbyssMinY = minY;
	while (IsRowSameValue(AbyssMinY, minX, maxX)) {
		AbyssMinY++;
	}
	AbyssMaxY = maxY;
	while (IsRowSameValue(AbyssMaxY, minX, maxX)) {
		AbyssMaxY--;
	}
	AbyssMinX = minX;
	while (IsColSameValue(AbyssMinX, minY, maxY)) {
		AbyssMinX++;
	}
	AbyssMaxX = maxX;
	while (IsColSameValue(AbyssMaxX, minY, maxY)) {
		AbyssMaxX--;
	}
	IsAbyssLit = IsLit(Point2D(minX, minY));
	IsAbyssSet = true;
	
	vector<Point2D> toDelete;
	for (auto& p : Dots) {
		if (p.GetX() < AbyssMinX || p.GetX() > AbyssMaxX
		|| p.GetY() < AbyssMinY || p.GetY() > AbyssMaxY) {
			toDelete.push_back(p);
		}
	}
	for (auto& p : toDelete) {
		auto iter = Dots.find(p);
		if (iter != Dots.end()) {
			Dots.erase(iter);
		}
	}
}

class ImageEnhancer
{
public:
	ImageEnhancer(const string& str) : Enhance(str) {
		assert(Enhance.size() == 512);
	}

	bool ShouldBeLit(int index) const {
		assert(index >= 0 && index < Enhance.size());
		return Enhance.at(index) == '#';
	}

private:
	string Enhance;
};

void Image::Print() const
{
	int minX = numeric_limits<int>::max();
	int maxX = numeric_limits<int>::min();

	int minY = numeric_limits<int>::max();
	int maxY = numeric_limits<int>::min();

	for (auto& p : Dots) {
		minX = min(p.GetX(), minX);
		maxX = max(p.GetX(), maxX);

		minY = min(p.GetY(), minY);
		maxY = max(p.GetY(), maxY);
	}
	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			Point2D p(x, y);
			if (IsLit(p)) {
				cout << "#";
			}
			else {
				cout << '.';
			}
		}
		cout << '\n';
	}
}

Image Image::Enhance(const ImageEnhancer& enhancer)
{
	Image newImage;

	int minX = numeric_limits<int>::max();
	int maxX = numeric_limits<int>::min();

	int minY = numeric_limits<int>::max();
	int maxY = numeric_limits<int>::min();

	if (IsAbyssSet)
	{
		minX = AbyssMinX - 5;
		maxX = AbyssMaxX + 5;
		minY = AbyssMinY - 5;
		maxY = AbyssMaxY + 5;
	}
	else 
	{
		for (auto& p : Dots) {
			minX = min(p.GetX(), minX);
			maxX = max(p.GetX(), maxX);

			minY = min(p.GetY(), minY);
			maxY = max(p.GetY(), maxY);
		}
		minX -= 5;
		minY -= 5;
		maxX += 5;
		maxY += 5;
	}
	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			Point2D p(x,y);
			int index = GetAreaAround(p);
			if (enhancer.ShouldBeLit(index))
			{
				newImage.Lit(p);
			}
		}
	}
	newImage.IdentifyAbyss();
	return newImage;
}

int partA(Image& image, ImageEnhancer& ie) {
	Image ni = image;
	for (int i = 0; i < 2; ++i) {
		ni = ni.Enhance(ie);
	}
	return ni.GetNumLit();
}

int partB(Image& image, ImageEnhancer& ie) {
	Image ni = image;
	for (int i = 0; i < 50; ++i) {
		ni = ni.Enhance(ie);
	}
	return ni.GetNumLit();
}



int main()
{
	string line;
	bool res = (bool)(getline(cin,line));
	assert(res);
	ImageEnhancer ie(line);
	Image image;
	res = (bool)(getline(cin, line));
	assert(res);
	assert(line.empty());
	int y = 0;
	while (getline(cin, line)) {
		for (int x = 0; x < line.size(); ++x) {
			if (line.at(x) == '#') {
				image.Lit(Point2D(x,y));
			}
		}
		++y;
	}
    std::cout << "Part A: " << partA(image, ie) << '\n';
	std::cout << "Part B: " << partB(image, ie) << '\n';
	return 0;
}
