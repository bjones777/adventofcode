#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

static int partA(const vector<string>& lines)
{
    int count = 0;
    for(auto& line : lines)
    {
        auto pos = line.find('|');
        string right = line.substr(pos+1);
        istringstream iss(right);
        string digits;
        while(iss >> digits) 
        {
         if(digits.size() == 2 || digits.size() == 3
         || digits.size() == 7 || digits.size() == 4) {
             ++count;
         }
        }
    }
    return count;
}

static int toIndex(char ch) {
    assert(ch >= 'a' && ch <= 'g');
    return ch - 'a';
}

static string trans(const string& segments, const array<char,7>& mapping)
{
    string newString;
    newString.reserve(segments.size());
    for(char ch : segments) {
        newString += mapping.at(toIndex(ch));
    }
    return newString;
}

static bool is0(const string& segments)
{
    return segments == "abcefg";
}

static bool is1(const string& segments)
{
    return segments == "cf";
}

static bool is2(const string& segments)
{
    return segments == "acdeg";
}

static bool is3(const string& segments)
{
    return segments == "acdfg";
}

static bool is4(const string& segments)
{
    return segments == "bcdf";
}

static bool is5(const string& segments)
{
    return segments == "abdfg";
}

static bool is6(const string& segments)
{
    return segments == "abdefg";
}

static bool is7(const string& segments)
{
    return segments == "acf";
}

static bool is8(const string& segments)
{
    return segments == "abcdefg";
}

static bool is9(const string& segments)
{
    return segments == "abcdfg";
}

static const int INVALID_DIGIT = -1;

static int to_digit(const string& segments, const array<char,7>& mapping)
{
    string newString = trans(segments, mapping);
    sort(newString.begin(), newString.end());
     if(is0(newString)) {
        return 0;
    }
    if(is1(newString)) {
        return 1;
    }
    if(is2(newString)) {
        return 2;
    }
    if(is3(newString)) {
        return 3;
    }
    if(is4(newString)) {
        return 4;
    }
    if(is5(newString)) {
        return 5;
    }
    if(is6(newString)) {
        return 6;
    }
    if(is7(newString)) {
        return 7;
    }
    if(is8(newString)) {
        return 8;
    }
    if(is9(newString)) {
        return 9;
    }
    return INVALID_DIGIT;
}

static bool is_valid(const string& segments, const array<char, 7>& mapping)
{
	return to_digit(segments, mapping) != INVALID_DIGIT;
}

bool all_valid(const vector<string>& digits, const array<char,7>& mapping)
{
    for(auto& digit : digits) 
    {
        if(!is_valid(digit, mapping))
        {
            return false;
        }
    }
    return true;
}

static vector<string> to_vector(const string& line) {
    istringstream iss(line);
    vector<string> retVal;
    string digits;
    while(iss >> digits) {
        if(digits == "|") continue;
        retVal.push_back(digits);
    }
    return retVal;
}

static int to_integer(const vector<string>& digits, const array<char,7>& mapping)
{
    int value = 0;
    for(auto& digit : digits) 
    {
        value *= 10;
        value += to_digit(digit, mapping);
    }
    return value;
}

int decode(const string& line)
{
    vector<string> allDigits = to_vector(line);
    vector<string> outputDigits = to_vector(line.substr(line.find('|')));

    array<char, 7> mapping = { 'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    do
    {
        if(all_valid(allDigits, mapping)) {
            return to_integer(outputDigits, mapping);
        }
    } while (next_permutation(mapping.begin(), mapping.end()));
    
    cerr << "bad line: " << line << '\n';
    assert(false);
    return -1;

}

static int partB(const vector<string>& lines)
{
    int sum = 0;
    for(auto& line : lines)
    {
        sum += decode(line);
    }
    return sum;
}


int main(int argc,char* argv[]) 
{
    vector<string> lines;
    string line;
    while(getline(cin, line)) {
        if(line.empty()) continue;
        lines.push_back(line);
    }
    cout << "Part A: " << partA(lines) << '\n';
    cout << "Part B: " << partB(lines) << '\n';
    return 0;
}