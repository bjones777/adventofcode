// Day01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

using namespace std;

int partA(const vector<int>& nums)
{
	int numIncreased = 0;
	for (int i = 1; i < (int)nums.size(); ++i)
	{
		if (nums[i] > nums[i - 1])
		{
			numIncreased++;
		}
	}
	return numIncreased;
}

int getWindow(const vector<int>& nums,int i)
{
	return nums.at(i-1) + nums.at(i) + nums.at(i+1);
}

int partB(const vector<int>& nums)
{
	int numIncreased = 0;
	for (int i = 2; i < (int)nums.size()-1; ++i)
	{
		if (getWindow(nums,i) > getWindow(nums,i - 1))
		{
			numIncreased++;
		}
	}
	return numIncreased;
}

int main()
{
    vector<int> nums;
	int num;
	while (cin >> num) {
		nums.push_back(num);
	}

	
	cout << partA(nums) << '\n';
	cout << partB(nums) << '\n';
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
