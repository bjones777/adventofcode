#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Board
{
public:
    Board()
    : Numbers(SIZE*SIZE,-1)
    , WasCalled(SIZE*SIZE, false) {
    }

    bool Read()
    {
        for(int i = 0;i < SIZE;++i) {
            for(int j = 0;j < SIZE;++j) {
                int x;
                if(cin >> x)
                {
                    Set(i,j,x);
                }
                else
                {
                    return false;
                }

            }
        }
        return true;
    }

    int Call(int v)
    {
        bool wasSet = false;
        for(int i = 0;i < SIZE;++i) {
            for(int j = 0;j < SIZE;++j) {
                if(Get(i,j) == v) {
                    WasCalled.at(i*SIZE+j) = true;
                    wasSet = true;
                }
            }
        }
        if(wasSet) 
        {
            return CalcBingo(v);
        }
        return -1;
    }

    
    void Set(int row, int col, int value) 
    {
        Numbers.at(row*SIZE + col) = value;
    }

    int Get(int row, int col) const 
    {
        return Numbers.at(row*SIZE + col);
    }

    bool GetCalled(int row, int col) const 
    {
        return WasCalled.at(row*SIZE + col);
    }


private:
    int CalcBingo(int v);
    int CalcBringRow(int v,int row) const;
    int CalcBringCol(int v,int row) const;
    int CalcBingoSE(int v) const;
    int CalcBingoNE(int v) const;
    int CalcUnmarkedScore() const;

    static const int SIZE = 5;
    vector<int> Numbers;
    vector<bool> WasCalled;
    bool HasWon = false;
};

int Board::CalcUnmarkedScore() const
{
    int sum = 0;
    for(int r = 0;r < SIZE;++r) 
    {
        for(int c = 0;c < SIZE;++c) 
        {
            if(!GetCalled(r,c)) 
            {
                sum += Get(r,c);
            }
        }
    }
    return sum;
}

int Board::CalcBingo(int v)
{
    if(HasWon) {
        return -1;
    }
    for(int r = 0;r < SIZE;++r) {
        int s = CalcBringRow(v, r);
        if(s != -1) {
            HasWon = true;
            return s;
        }
    }

    for(int c = 0;c < SIZE;++c) {
        int s = CalcBringCol(v, c);
        if(s != -1) {
            HasWon = true;
            return s;
        }
    }
    return -1;
}

int Board::CalcBringRow(int v,int row) const
{
    for(int c = 0;c < SIZE;++c) {
        if(!GetCalled(row,c)) {
            return -1;
        }
    }
    return CalcUnmarkedScore() * v;
}
int Board::CalcBringCol(int v,int col) const
{
    for(int r = 0;r < SIZE;++r) {
        if(!GetCalled(r,col)) {
            return -1;
        }
    }
    return CalcUnmarkedScore() * v;

}
int Board::CalcBingoSE(int v) const
{
    for(int r = 0;r < SIZE;++r) {
        if(!GetCalled(r,r)) {
            return -1;
        }
    }
    return CalcUnmarkedScore() * v;
}

int Board::CalcBingoNE(int v) const
{
    for(int r = 0;r < SIZE;++r) {
        if(!GetCalled(SIZE-r-1,r)) {
            return -1;
        }
    }
    return CalcUnmarkedScore() * v;
}

vector<int> toVector(const string& s)
{
    istringstream iss(s);
    vector<int> retVal;
    int x;
    char spacer;
    while(iss >> x)
    {
        retVal.push_back(x);
        if(iss >> spacer) {
            assert(spacer == ',');
        }
    }
    return retVal;
}

int partA(const vector<int>& calls,vector<Board> boards)
{
    for(int call : calls) {
        for(auto& b : boards) {
            int s = b.Call(call);
            if(s != -1) {
                return s;
            }
        }
    }
    return -1;
}

int partB(const vector<int>& calls,vector<Board> boards)
{
    int lastScore = -1;
    for(int call : calls) {
        for(auto& b : boards) {
            int s = b.Call(call);
            if(s != -1) {
                lastScore = s;
            }
        }
    }
    return lastScore;
}

int main(int argc,char* argv[]) 
{
    string line;
    cin >> line;
    vector<Board> boards;
    Board b;
    while(b.Read())
    {
        boards.push_back(b);
    }
    cout << "Boards " << boards.size() << '\n';

    vector<int> calls = toVector(line);
    cout << "Part A: " << partA(calls, boards) << '\n';
    cout << "Part B: " << partB(calls, boards) << '\n';
 
    return 0;
}