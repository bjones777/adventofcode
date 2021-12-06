#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Laternfish
{
public:
    static const int NUM_DAYS = 7;
    Laternfish(int inTimer = NUM_DAYS + 1)
    : timer(inTimer)
    {
    }

    bool Tick();

    int GetTimer() const { return timer; }
private:
    int timer;
};

bool Laternfish::Tick()
{
    timer -= 1;
    if(timer < 0)
    {
        timer = NUM_DAYS - 1;
        return true;
    }
    return false;
}



vector<Laternfish> toVector(const string& s)
{
    istringstream iss(s);
    vector<Laternfish> retVal;
    int x;
    char spacer;
    while(iss >> x)
    {
        retVal.emplace_back(x);
        if(iss >> spacer) {
            assert(spacer == ',');
        }
    }
    return retVal;
}

static int partA(vector<Laternfish> fishes)
{
    for(int i = 0;i < 80;++i) {
        int count = 0;
        for(auto& fish : fishes) {
            if(fish.Tick())
            {
                ++count;
            }
        }
        while(count-- > 0) {
            fishes.emplace_back(Laternfish{});
        }
    }
    return (int)fishes.size();
}

static int64_t partB(vector<Laternfish> fishes)
{
    vector<int64_t> fishToLifetime(9);
    for(auto& fish : fishes) {
        fishToLifetime.at(fish.GetTimer()) += 1;
    }
    for(int i = 0;i < 256;++i) {
        vector<int64_t> newFishToLifetime(9);
        for(int j = 1;j < (int)fishToLifetime.size();++j) {
            newFishToLifetime.at(j-1) = fishToLifetime[j];
        }
        newFishToLifetime.at(Laternfish::NUM_DAYS-1) += fishToLifetime[0];
        newFishToLifetime.at(Laternfish::NUM_DAYS+1) += fishToLifetime[0];
        fishToLifetime.swap(newFishToLifetime); 
    }
    int64_t count = 0;
    for(int i = 0;i < (int)fishToLifetime.size();++i) {
        count += fishToLifetime[i];
    }
    return count;
}



int main(int argc,char* argv[]) 
{
    string line;
    cin >> line;
    vector<Laternfish> fishes = toVector(line);
    cout << "Part A: " << partA(fishes) << '\n';
    cout << "Part B: " << partB(fishes) << '\n';
    return 0;
}