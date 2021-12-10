#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

static int get_value(char ch)
{
    if(ch == ')')
    {
        return 3;
    }
    else if(ch == ']') 
    {
        return 57;
    }
    else if(ch == '}')
    {
        return 1197;
    }
    assert(ch == '>');
    return 25137;
}

static int get_value2(char ch)
{
    if(ch == ')')
    {
        return 1;
    }
    else if(ch == ']') 
    {
        return 2;
    }
    else if(ch == '}')
    {
        return 3;
    }
    assert(ch == '>');
    return 4;
}

static char get_close(char ch)
{
    if(ch == '(')
    {
        return ')';
    }
    else if(ch == '[') 
    {
        return ']';
    }
    else if(ch == '{')
    {
        return '}';
    }
    else if(ch == '<') 
    {
        return '>';
    }
    return 0;
}

static int process_line(const string& line)
{
    vector<char> expected; 
    for(char ch : line)
    {
        char close = get_close(ch);
        if(close == 0) 
        {
            if(expected.empty() || expected.back() != ch) {
                return get_value(ch);
            }
            
            expected.pop_back();
        }
        else {
            expected.push_back(close);
        }
    }
    //assert(expected.empty());
    return 0;
}

static bool process_line2(const string& line,int64_t& score)
{
    vector<char> expected; 
    for(char ch : line)
    {
        char close = get_close(ch);
        if(close == 0) 
        {
            if(expected.empty() || expected.back() != ch) {
                return false;
            } 
            expected.pop_back();
        }
        else {
            expected.push_back(close);
        }
    }
    int64_t totalScore = 0;
    for(auto i = expected.rbegin();i != expected.rend();++i) {
        totalScore *= 5;
        totalScore += get_value2(*i);
    }
    score = totalScore;
    return true;
}

static int partA(const vector<string>& lines)
{
    int sum = 0;
    for(const string& line : lines) {
        sum += process_line(line);
    }
    return sum;
}

static int64_t partB(const vector<string>& lines)
{
    vector<int64_t> scores;
    for(const string& line : lines) {
        int64_t score = 0;
        if(process_line2(line, score)) {
            scores.push_back(score);
        }
    }
    sort(scores.begin(), scores.end());
    return scores.at(scores.size()/2);
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