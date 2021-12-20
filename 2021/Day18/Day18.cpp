// Day18.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

enum class NumberState
{
	Value,
	Pair,
};

struct Snailnumber
{
public:
	Snailnumber(const string& str);
	Snailnumber(istream& is);
	~Snailnumber();

	Snailnumber* Clone() {
		Snailnumber* clone = new Snailnumber();
		for (int i = 0; i < NumberStates.size(); ++i) {
			clone->NumberStates[i] = NumberStates[i];
			if (NumberStates[i] == NumberState::Value) {
				clone->Values[i] = Values[i];
			}
			else {
				clone->Pairs[i] = Pairs[i]->Clone();
				clone->Pairs[i]->SetParent(clone);
			}
		}
		return clone;
	}

	static Snailnumber* Add(Snailnumber* s1,Snailnumber* s2);
	int64_t GetMagnitude() const;

	Snailnumber* GetParent() const { return parent; }
	void SetParent(Snailnumber* p) { 
		assert(parent == nullptr);
		parent = p; 
	}

	void Print(ostream& os) const;
private:
	Snailnumber()
	{}

	Snailnumber(Snailnumber* s0, Snailnumber* s1)
	{
		NumberStates[0] = NumberState::Pair;
		Pairs[0] = s0;
		NumberStates[1] = NumberState::Pair;
		Pairs[1] = s1;
	}

	Snailnumber(int64_t v0, int64_t v1)
	{
		NumberStates[0] = NumberState::Value;
		Values[0] = v0;
		NumberStates[1] = NumberState::Value;
		Values[1] = v1;
	}

	void Reduce();
	bool IsLeftChild(Snailnumber* child) const
	{
		if (NumberStates[0] != NumberState::Pair)
		{
			return false;
		}
		return Pairs[0] == child;
	}

	bool IsRightChild(Snailnumber* child) const
	{
		if (NumberStates[1] != NumberState::Pair)
		{
			return false;
		}
		return Pairs[1] == child;
	}
	
	void Explode();
	void DoLeftExplode();
	void DoRightExplode();
	bool ExplodeIfNecessary(int depth);
	bool SplitIfNecessary();
	int64_t GetMagnitudeAtIndex(int index) const;
	void initAtIndex(istream& is, int index);
	array<NumberState,2> NumberStates;
	array<int64_t,2> Values;
	array<Snailnumber*,2> Pairs;
	Snailnumber* parent = nullptr;
};

void Snailnumber::Print(ostream& os) const
{
	os << '[';
	for (int i = 0; i < NumberStates.size(); ++i) {
		if(i != 0) os << ',';
		if (NumberStates[i] == NumberState::Pair) {
			Pairs[i]->Print(os);
		}
		else {
			os << Values[i];
		}
		
	}
	os << ']';
}


Snailnumber* Snailnumber::Add(Snailnumber* s1, Snailnumber* s2)
{
	Snailnumber* retVal = new Snailnumber(s1,s2);
	s1->SetParent(retVal);
	s2->SetParent(retVal);
	retVal->Reduce();
	return retVal;
}

Snailnumber::~Snailnumber()
{
	for (int i = 0; i < NumberStates.size(); ++i) {
		if (NumberStates[i] == NumberState::Pair) {
			delete Pairs[i];
		}
	}
}

void Snailnumber::Reduce()
{
	//cout << "After addition: ";
	//Print(cout);
	//cout << '\n';
	for (;;)
	{
		bool res = ExplodeIfNecessary(0);
		if(res) {
			//cout << "After explode: ";
			//Print(cout);
			//cout << '\n';
			continue;
		}
		res = SplitIfNecessary();
		if (!res) {
			return;
		}
		else {
			//cout << "After split: ";
			//Print(cout);
			//cout << '\n';
		}
	}
}

void Snailnumber::DoLeftExplode()
{
	Snailnumber* current = this;
	Snailnumber* localParent = GetParent();
	while (localParent != nullptr)
	{
		if (localParent->IsRightChild(current)) {
			if (localParent->NumberStates[0] != NumberState::Pair)
			{
				localParent->Values[0] += Values[0];
				return;
			}
			localParent = localParent->Pairs[0];
			while (localParent->NumberStates[1] == NumberState::Pair)
			{
				localParent = localParent->Pairs[1];
			}
			localParent->Values[1] += Values[0];
			return;
		}
		else {
			current = localParent;
			localParent = localParent->GetParent();
		}
	}
}

void Snailnumber::DoRightExplode()
{
	Snailnumber* current = this;
	Snailnumber* localParent = GetParent();
	while (localParent != nullptr)
	{
		if (localParent->IsLeftChild(current)) {
			if (localParent->NumberStates[1] != NumberState::Pair)
			{
				localParent->Values[1] += Values[1];
				return;
			}
			localParent = localParent->Pairs[1];
			while (localParent->NumberStates[0] == NumberState::Pair)
			{
				localParent = localParent->Pairs[0];
			}
			localParent->Values[0] += Values[1];
			return;
		}
		else {
			current = localParent;
			localParent = localParent->GetParent();
		}
	}
}

void Snailnumber::Explode()
{
	for (int i = 0; i < NumberStates.size(); ++i) {
		assert(NumberStates[i] == NumberState::Value);
	}
	assert(parent != nullptr);
	DoLeftExplode();
	DoRightExplode();
	if (parent->IsLeftChild(this))
	{
		parent->NumberStates[0] = NumberState::Value;
		parent->Values[0] = 0;
		delete this;
		return;
	}
	assert(parent->IsRightChild(this));
	parent->NumberStates[1] = NumberState::Value;
	parent->Values[1] = 0;
	delete this;
}


bool Snailnumber::ExplodeIfNecessary(int depth)
{
	if (depth == 4)
	{
		Explode();
		return true;
	}
	for (int i = 0; i < NumberStates.size(); ++i) {
		if (NumberStates[i] == NumberState::Pair) {
			if (Pairs[i]->ExplodeIfNecessary(depth+1)) {
				return true;
			}
		}
	}
	return false;

}

bool Snailnumber::SplitIfNecessary()
{
	for (int i = 0; i < NumberStates.size(); ++i) {
		if (NumberStates[i] == NumberState::Pair) {
			if (Pairs[i]->SplitIfNecessary()) {
				return true;
			}
		}
		else {
			assert(NumberStates[i] == NumberState::Value);
			if (Values[i] >= 10) {
				int64_t v0 = Values[i]/2;
				int64_t v1 = v0 + Values[i] % 2;
				NumberStates[i] = NumberState::Pair;
				Pairs[i] = new Snailnumber(v0,v1);
				Pairs[i]->SetParent(this);
				return true;
			}
		}
	}
	return false;
}

int64_t Snailnumber::GetMagnitudeAtIndex(int index) const
{
	if (NumberStates[index] == NumberState::Value) {
		return Values[index];
	}
	return Pairs[index]->GetMagnitude();
}

int64_t Snailnumber::GetMagnitude() const
{
	int64_t retVal = 0;
	retVal += 3 * GetMagnitudeAtIndex(0);
	retVal += 2 * GetMagnitudeAtIndex(1);
	return retVal;
}

void Snailnumber::initAtIndex(istream& is,int index)
{
	char ch;
	bool isGood;
	isGood = (bool)(is >> ch);
	assert(isGood);
	if (ch == '[')
	{
		is.putback(ch);
		NumberStates[index] = NumberState::Pair;
		Pairs[index] = new Snailnumber(is);
		Pairs[index]->SetParent(this);
	}
	else
	{
		int64_t val;
		is.putback(ch);
		is >> val;
		NumberStates[index] = NumberState::Value;
		Values[index] = val;
	}
}

Snailnumber::Snailnumber(istream& is)
{
	char ch;
	bool isGood;
	isGood = (bool)(is >> ch);
	assert(isGood);
	assert(ch == '[');
	for (int i = 0; i < NumberStates.size(); ++i) {
		if (i != 0)
		{
			isGood = (bool)(is >> ch);
			assert(isGood);
			assert(ch == ',');
		}
		initAtIndex(is,i);
	}
	isGood = (bool)(is >> ch);
	assert(isGood);
	assert(ch == ']');
}

Snailnumber::Snailnumber(const string& str)
{
	istringstream is(str);
	char ch;
	bool isGood;
	isGood = (bool)(is >> ch);
	assert(isGood);
	assert(ch == '[');
	for (int i = 0; i < NumberStates.size(); ++i) {
		if (i != 0)
		{
			isGood = (bool)(is >> ch);
			assert(isGood);
			assert(ch == ',');
		}
		initAtIndex(is, i);
	}
	isGood = (bool)(is >> ch);
	assert(isGood);
	assert(ch == ']');

}


static void testA() {
	Snailnumber* a = new Snailnumber("[[[[4,3],4],4],[7,[[8,4],9]]]");
	Snailnumber* b = new Snailnumber("[1,1]");
	Snailnumber* sum = Snailnumber::Add(a,b);
	
	sum->Print(cout);
	delete sum;
}

static int64_t partA(const vector<Snailnumber*>& inNumbers) {
	vector<Snailnumber*> numbers;
	numbers.reserve(inNumbers.size());
	for(auto ri = inNumbers.rbegin();ri != inNumbers.rend();++ri) {
		numbers.push_back((*ri)->Clone());
	}
	Snailnumber* sum = numbers.back();
	numbers.pop_back();
	while (!numbers.empty()) {
		sum = Snailnumber::Add(sum, numbers.back());
		numbers.pop_back();
	}
	int64_t val = sum->GetMagnitude();
	delete sum;
	return val;
}

static int64_t partB(const vector<Snailnumber*>& numbers) {
	int64_t maxMag = numeric_limits<int64_t>::min();
	for (int i = 0; i < numbers.size(); ++i) {
		for (int j = 0; j < numbers.size(); ++j) {
			if(i == j) continue;
			Snailnumber* sum = Snailnumber::Add(numbers[i]->Clone(), numbers[j]->Clone());
			maxMag = max(maxMag, sum->GetMagnitude());
			delete sum;
		}
	}
	return maxMag;
}

int main()
{
	//testA();
	vector<Snailnumber*> numbers;
	string line;
	while (getline(cin, line)) {
		if(line.empty()) continue;
		numbers.push_back(new Snailnumber(line));
	}
	cout << "Part A: " << partA(numbers) << '\n';
	cout << "Part B: " << partB(numbers) << '\n';

	while (!numbers.empty()) {
		delete numbers.back();
		numbers.pop_back();
	}
	return 0;
}

