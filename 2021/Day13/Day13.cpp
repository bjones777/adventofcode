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


class Paper
{
public:
    void AddDot(const Point2D& p)
    {
        Dots.insert(p);
    }

    void FoldX(int x);
    void FoldY(int y);

    int GetNumDots() const { return (int)Dots.size(); }
    void Print() const;
private:
    unordered_set<Point2D> Dots;
};

void Paper::FoldX(int x)
{
    unordered_set<Point2D> newDots;
    for(auto& dot : Dots) {
        Point2D newDot{x - abs(x - dot.GetX()), dot.GetY()};
        newDots.insert(newDot);
    }
    newDots.swap(Dots);
}

void Paper::Print() const
{
    int minX = numeric_limits<int>::max();
    int maxX = numeric_limits<int>::min();

    int minY = numeric_limits<int>::max();
    int maxY = numeric_limits<int>::min();

    for(auto& p : Dots) {
        minX = min(p.GetX(), minX);
        maxX = max(p.GetX(), maxX);

        minY = min(p.GetY(), minY);
        maxY = max(p.GetY(), maxY);
    } 

    for(int y = minY; y <= maxY;++y) {
        for(int x = minX; x <= maxX;++x) {
            Point2D p{x,y};
            auto i = Dots.find(p);
            if(i != Dots.end()) {
                cout << "#";
            }
            else {
                cout << ' ';
            }
        }
        cout << '\n';
    }
}

void Paper::FoldY(int y)
{
    unordered_set<Point2D> newDots;
    for(auto& dot : Dots) {
        Point2D newDot{dot.GetX(), y - abs(y- dot.GetY())};
        newDots.insert(newDot);
    }
    newDots.swap(Dots);
}

Point2D read_dot(const string& line)
{
    int x, y;
    char ch;
    istringstream iss(line);
    if(iss >> x >> ch >> y)
    {
        assert(ch == ',');
        return Point2D(x,y);
    }
    cerr << "Bad Line: " << line << '\n';
    throw runtime_error("bad line");
}

void read_fold(Paper& paper, const string& line) {
 //   paper.Print();
    size_t pos = line.find('=');
    char ch = line.at(pos-1);
    if(ch == 'x')
    {
        string right = line.substr(pos+1);
        paper.FoldX(atoi(right.c_str()));
    }
    else 
    {
        assert(ch == 'y');
        string right = line.substr(pos+1);
        paper.FoldY(atoi(right.c_str()));
    }
}

static int partA(const Paper& paper) {
    return paper.GetNumDots();
}

int main(int argc,char* argv[]) 
{
    Paper paper;
    string line;
    bool isFirstFold = true;
    while(getline(cin, line))
    {
        if(line.empty()) continue;
        size_t pos = line.find(',');
        if(pos != string::npos) {
            paper.AddDot(read_dot(line));
        }    
        else {
            read_fold(paper, line);
            if(isFirstFold)
            {
                cout << "Part A: " << paper.GetNumDots() << '\n';
                isFirstFold = false;
            }
        }
    }
	cout << "Part B:\n";
    paper.Print();
    return 0;
}