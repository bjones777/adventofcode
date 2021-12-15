#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <queue>
#include <string>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

class Grid
{
public:
    Grid(istream& is);

    int GetWidth() const { return Width; }
    int GetHeight() const { return Height; }

    int GetLowestRiskPath() const;
    void Enlarge();
private:
    int GetIndex(int row,int col) const { return row * Width + col; }
    int GetRiskValue(int row, int col) const { return RiskValues.at(GetIndex(row,col)); }
    int Width;
    int Height;
    vector<int> RiskValues;
};

void Grid::Enlarge()
{
    int NewWidth = Width*5;
    int NewHeight = Height*5;
    vector<int> NewRiskValues;
    NewRiskValues.reserve(NewWidth*NewHeight);
    for(int r = 0; r < NewWidth;++r)
    {
        int NumTop = r/Width;
        int OrigR = r % Width;
        for(int c = 0; c < NewHeight;++c) {
            int NumRight = c/Height;
            int OrigC = c % Height;
            int ToAdd =  NumTop + NumRight;
            int NewRisk = (GetRiskValue(OrigR, OrigC)-1 + ToAdd) % 9 + 1;

            NewRiskValues.push_back(NewRisk);
        }
    }
    Width = NewWidth;
    Height = NewHeight;
    RiskValues.swap(NewRiskValues);
}

struct Entry
{
    Entry(int inRow, int inCol, int inTotalRisk) :
    Row(inRow), Col(inCol), TotalRisk(inTotalRisk) {}

    int Row;
    int Col;
    int TotalRisk;

    bool operator<(const Entry& entry) const 
    {
        return TotalRisk > entry.TotalRisk;
    }
};

int Grid::GetLowestRiskPath() const
{
    priority_queue<Entry> q;
    vector<bool> Visited(RiskValues.size());
    q.push(Entry{0,0,0});
    while(!q.empty()) {
        Entry e = q.top();
        q.pop();
        if(Visited.at(GetIndex(e.Row, e.Col))) continue;
        if(e.Row == Height-1 && e.Col == Width-1) {
            return e.TotalRisk;
        }
        Visited.at(GetIndex(e.Row, e.Col)) = true;
        for(int i = 0;i < 4;++i) {
            int nr = e.Row;
            int nc = e.Col;
            switch(i)
            {
                case 0:
                    nr += 1;
                    break;
                case 1:
                    nr -= 1;
                    break;
                case 2:
                    nc += 1;
                    break;
                case 3:
                    nc -= 1;
                    break;
            }
            if(nr < 0 || nr >= Height) continue;
            if(nc < 0 || nc >= Width) continue;
            if(Visited.at(GetIndex(nr,nc))) continue;
            q.push(Entry{nr,nc,e.TotalRisk+GetRiskValue(nr,nc)});
        }
    }
    throw runtime_error("no path");
}

Grid::Grid(istream& is)
: Width(-1), Height(-1)
{
    string line;
    while(getline(is,line))
    {
        if(line.empty()) continue;
        if(Width == -1) {
            Width = static_cast<int>(line.size());
        } 
        else {
            assert(Width == line.size());
        }
        for(char ch : line) {
            RiskValues.push_back(ch - '0');
        }
    }
    Height = static_cast<int>(RiskValues.size()) / Width;
    assert(Height*Width == RiskValues.size());
}

static int partA(Grid& grid) {
    return grid.GetLowestRiskPath();
}

static int partB(Grid& grid) {
    grid.Enlarge();
    return grid.GetLowestRiskPath();
}

int main(int argc,char* argv[]) 
{
    Grid grid(cin);
    cout << "Part A: " << partA(grid) << '\n';
    cout << "Part B: " << partB(grid) << '\n';
    return 0;
}