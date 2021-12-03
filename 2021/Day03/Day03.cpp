#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int64_t bin2Dec(const string& s)
{
    int64_t res = 0;
    int numBits = static_cast<int>(s.size());
    for(int i = 0;i < numBits;++i) {
        res *= 2;
        if(s[i] == '1')
        {
            res += 1;
        }
    }
    return res;
}

static int64_t partA(const vector<string>& allBits)
{
    int numBits = static_cast<int>(allBits.front().size());
    string gamma;
    string episilon;
    for(int i = 0;i < numBits; ++i)
    {
        int numZeros = 0;
        int numOnes = 0;
        for(auto& bits : allBits) 
        {
            if(bits.at(i) == '0')
            {
                numZeros+= 1;
            }
            else 
            {
                assert(bits.at(i) == '1');
                numOnes += 1;
            }
        }
        if(numZeros > numOnes)
        {
            gamma.push_back('0');
            episilon.push_back('1');
        }
        else 
        {
            assert(numZeros != numOnes);
            gamma.push_back('1');
            episilon.push_back('0');
        }
    }
    //cout << gamma << ' ' << bin2Dec(gamma) <<  '\n';
    //cout << episilon << ' ' << bin2Dec(episilon) << '\n';
    return bin2Dec(gamma) * bin2Dec(episilon);
}

vector<string> filterByBit(const vector<string>& allBits, int bitNum, bool mostOrLeast)
{
    int numZeros = 0;
    int numOnes = 0;    
    for(auto& bits : allBits) 
    {
        if(bits.at(bitNum) == '0')
        {
            numZeros+= 1;
        }
        else 
        {
            assert(bits.at(bitNum) == '1');
            numOnes += 1;
        }
    }
    if(numZeros == 0 || numOnes == 0)
    {
        return allBits;
    }
    if(numZeros > numOnes)
    {
        vector<string> retVal;
        if(mostOrLeast)
        {
            for(auto& bits : allBits) 
            {
                if(bits.at(bitNum) == '0') {
                    retVal.push_back(bits);
                }
            }    
        }
        else 
        {
            for(auto& bits : allBits) 
            {
                if(bits.at(bitNum) == '1') {
                    retVal.push_back(bits);
                }
            }
        }
        return retVal;
    }
    else if(numOnes > numZeros)
    {
        vector<string> retVal;
        if(mostOrLeast)
        {
            for(auto& bits : allBits) 
            {
                if(bits.at(bitNum) == '1') {
                    retVal.push_back(bits);
                }
            }    
        }
        else 
        {
            for(auto& bits : allBits) 
            {
                if(bits.at(bitNum) == '0') {
                    retVal.push_back(bits);
                }
            }
        }
        return retVal;
    }
    else 
    {
        vector<string> retVal;
        if(mostOrLeast)
        {
            for(auto& bits : allBits) 
            {
                if(bits.at(bitNum) == '1') {
                    retVal.push_back(bits);
                }
            }    
        }
        else 
        {
            for(auto& bits : allBits) 
            {
                if(bits.at(bitNum) == '0') {
                    retVal.push_back(bits);
                }
            }
        }
        return retVal;
    }
}

static int64_t partB(const vector<string>& allBits)
{
    vector<string> allBitsOxygen(allBits);
    int bitNum = 0;
    while(allBitsOxygen.size() != 1) 
    {
        allBitsOxygen = filterByBit(allBitsOxygen, bitNum, true);
        ++bitNum;
    }

    bitNum = 0;
    vector<string> allBitsCo2(allBits);
    while(allBitsCo2.size() != 1) 
    {
        allBitsCo2 = filterByBit(allBitsCo2, bitNum, false);
        ++bitNum;
    }
    return bin2Dec(allBitsOxygen.front()) * bin2Dec(allBitsCo2.front());
}

int main(int argc,char* argv[]) 
{
    vector<string> allBits;
    string bits;
    while(cin >> bits) 
    {
        allBits.push_back(bits);
    }
    cout << "Part A: " << partA(allBits) << '\n';
    cout << "Part B: " << partB(allBits) << '\n';

    return 0;
}