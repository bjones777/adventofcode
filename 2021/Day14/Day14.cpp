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

struct EntryKey
{
	EntryKey(char inLeft, char inRight, int inDepth, char inCh) 
	: left(inLeft), right(inRight), depth(inDepth), ch(inCh) {}

	char left;
	char right;
	int depth;
	char ch;

	bool operator==(const EntryKey& rhs) const {
		return left == rhs.left && right == rhs.right && depth == rhs.depth && ch == rhs.ch;
	}
};
namespace std {

	template <>
	struct hash<EntryKey>
	{
		std::size_t operator()(const EntryKey& k) const
		{
			return (hash<char>()(k.left)
				^ ((hash<char>()(k.right) << 1) >> 1)
				^ ((hash<char>()(k.ch) << 2) >> 2)
				^ ((hash<int>()(k.depth) << 3) >> 3));
		}
	};
}


class Polymer
{
public:
	Polymer(const string& inTemplate) : Template(inTemplate), Current(inTemplate) {}

    void AddRule(const string& rhs, const string& lhs)
    {
        Rules.insert(make_pair(rhs,lhs));
    }

    void Step();

    const string& GetCurrent() const { return Current; }
	const string& GetTemplate() const { return Template; }
	unordered_set<char> GetAllLetters() const;

	int64_t GetLetterFrequencyAtDepth(char left, char right, int depth, char desired);
private:
	string Template;
	string Current;
    unordered_map<string,string> Rules;
	unordered_map<EntryKey, int64_t> LetterFreq;
};

unordered_set<char> Polymer::GetAllLetters() const
{
	unordered_set<char> retVal;
	for (char ch : Template) {
		retVal.insert(ch);
	}
	for (auto& p : Rules) {
		for (char ch : p.first) {
			retVal.insert(ch);
		}
		for (char ch : p.second) {
			retVal.insert(ch);
		}
	}
	return retVal;
}

int64_t Polymer::GetLetterFrequencyAtDepth(char left, char right, int depth, char desired)
{
	if (depth == 0) {
		return 0;
	}
	EntryKey entryKey(left, right, depth, desired);
	auto letterIter = LetterFreq.find(entryKey);
	if(letterIter != LetterFreq.end()) {
		return letterIter->second;
	}
	int64_t retVal = 0;
	string ab;
	ab += left;
	ab += right;
	auto iter = Rules.find(ab);
	if (iter == Rules.end()) {
		LetterFreq.insert(make_pair(entryKey, 0));
		return 0;
	}
	if(desired == iter->second.front()) 
	{
		retVal += 1;
	}
	retVal += GetLetterFrequencyAtDepth(left, iter->second.front(), depth-1, desired);
	retVal += GetLetterFrequencyAtDepth(iter->second.front(), right, depth - 1, desired);
	LetterFreq.insert(make_pair(entryKey, retVal));
	return retVal;
}

void Polymer::Step() {
    string nextString;
    for(int i = 1;i < (int)Current.size();++i) {
        nextString += Current.at(i-1);
        string small = Current.substr(i-1,2);
        auto entry = Rules.find(small);
        if(entry != Rules.end()) {
            nextString += entry->second;
        }
    }
    nextString += Current.back();
    Current.swap(nextString);
}


struct MyFreq
{
    MyFreq(char ch, int freq) : Ch(ch), Freq(freq) {} 
    char Ch;
    int Freq;

    bool operator<(const MyFreq& f) const {
        return Ch < f.Ch;
    }
};

static pair<int,int> determineMostLeast(const Polymer& p)
{
    unordered_map<char, int> freq;
    for(char ch : p.GetCurrent()) {
        freq[ch] += 1;
    }
 
    int mostFrequent = numeric_limits<int>::min();
    int leastFrequent = numeric_limits<int>::max();
    for(auto& f : freq) {
        mostFrequent = max(mostFrequent,f.second);
        leastFrequent = min(leastFrequent, f.second);
    }
    return make_pair(mostFrequent, leastFrequent);
}

static int partA(Polymer& p) {
    for(int i = 0;i < 10;++i) {
        p.Step();
    }
    auto v = determineMostLeast(p);
    return v.first - v.second;
}

static int64_t partB(Polymer& p) {
	unordered_set<char> allLetters = p.GetAllLetters();
	string t = p.GetTemplate();

	unordered_map<char, int64_t> freq;
	for (char ch : t)
	{
		freq[ch] += 1;
	}
	for (int i = 1; i < (int)t.size(); ++i) {
		for (char ch : allLetters) {
			freq[ch] += p.GetLetterFrequencyAtDepth(t[i - 1], t[i], 40, ch);
		}
	}
	int64_t mostFrequent = numeric_limits<int64_t>::min();
	int64_t leastFrequent = numeric_limits<int64_t>::max();
	for (auto& f : freq) {
		mostFrequent = max(mostFrequent, f.second);
		leastFrequent = min(leastFrequent, f.second);
	}
	return mostFrequent - leastFrequent;
}


int main(int argc,char* argv[]) 
{
    string line;
    bool result = (bool)getline(cin, line);
    assert(result);
    Polymer polymer{line};
    result = (bool)getline(cin, line);
    assert(result);
    assert(line.empty());

    while(getline(cin, line))
    {
        if(line.empty()) continue;
        size_t pos = line.find('-');
        assert(pos != string::npos);
        string lhs = line.substr(0, pos-1);
        string rhs = line.substr(pos + 3);
        polymer.AddRule(lhs, rhs);    
    }
    Polymer p1 = polymer;
    cout << "Part A: " << partA(p1) << '\n';
    cout << "Part B: " << partB(polymer) << '\n';
    return 0;
}