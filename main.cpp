#include "GDP.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono; 
// Declaring Functions
void mergeSortTable(vector<GDP>& table);
void mergeVector(vector<GDP>& left, vector<GDP>& right, vector<GDP>& table);

// Merge Sort Function
void mergeSortTable(vector<GDP>& table) {
	if (table.size() <= 1) {
		return;
	}
	int midIndex = table.size() / 2;
	vector<GDP> left;
	vector<GDP> right;

	for (int i = 0; i < midIndex; ++i) {
		left.push_back(table[i]);
	}
	for (int i = midIndex; i < table.size(); ++i) {
		right.push_back(table[i]);
	}
	mergeSortTable(left);
	mergeSortTable(right);
	mergeVector(left, right, table);
}
void mergeVector(vector<GDP>& left, vector<GDP>& right, vector<GDP>& table) {
	int leftSize = left.size(), rightSize = right.size();
	int leftIndex = 0, rightIndex = 0, mergedIndex = 0;

	while ((leftIndex < leftSize) && (rightIndex < rightSize)) {
		if (left[leftIndex] > right[rightIndex]) {
			table[mergedIndex++] = left[leftIndex++];
		}
		else if (left[leftIndex] < right[rightIndex]) {
			table[mergedIndex++] = right[rightIndex++];
		}
	}
	while (leftIndex < leftSize) {
		table[mergedIndex++] = left[leftIndex++];
	}
	while (rightIndex < rightSize) {
		table[mergedIndex++] = right[rightIndex++];
	}
}
// Heap Sort Function

int main() {
	vector<GDP> GDPTable; 
	GDPTable.push_back(GDP(1981, "Food", "LA", 102030));
	GDPTable.push_back(GDP(1976, "Sports", "AZ", 112321));
	GDPTable.push_back(GDP(1993, "Education", "MA", 165456));
	GDPTable.push_back(GDP(2001, "Clothing", "CA", 218205));
	GDPTable.push_back(GDP(2013, "Travel", "FL", 1551));
	GDPTable.push_back(GDP(1998, "Farm", "NE", 25));
	GDPTable.push_back(GDP(1998, "Cow", "NE", 2512312));
	GDPTable.push_back(GDP(1995, "Cow", "NE", 2512312));
	GDPTable.push_back(GDP(1995, "Cow", "SC", 2512312));
	GDPTable.push_back(GDP(1995, "Cattle", "SC", 2512312));
	auto start = high_resolution_clock::now();
	mergeSortTable(GDPTable);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << duration.count() << endl;

	if (GDPTable[1] > GDPTable[6]) {
		cout << GDPTable[1].getName() << " in " << GDPTable[1].getYear() << " is bigger" << endl;
	}
	return 0;
}