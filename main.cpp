#include "GDP.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <fstream>
#include <sstream>
#include <stack>
#include <set>

using namespace std;
using namespace std::chrono; 

// Declaring Functions
void mergeSort3WayTable(vector<GDP>& table); // works
void mergeVector(vector<GDP>& arr1, vector<GDP>& arr2, vector<GDP>& arr3, vector<GDP>& table); // works
void readCSV(string fileName, vector<GDP>& table); // works (remove the shortened thing for testing when done)
void quickSortTable(vector<GDP>& table); // works
int partitionTable(vector<GDP>& table, int start, int end); // works
void findGDP(vector<GDP>& table, vector<GDP>& found, string state, int year);
void sectionGDP(vector<GDP>& table, vector<GDP>& result, int x, int option, bool uniqueName, bool uniqueState, bool uniqueYear, bool selectState, string stateName, int startYear, int endYear);
void printGDPStateYear(vector<GDP>& table);
void printMenu();
void printGDPNode(GDP current);

// Defining functions
// CSV Reader
void readCSV(string fileName, vector<GDP>& table) {
	int testNum = 0;
	ifstream inFile(fileName);
	string dataLine;
	// Looping through the CSV and grabbing data
	while (getline(inFile, dataLine)) {
		istringstream stream(dataLine);
		string smallData;
		// using istringstream to grab each bit of the line from the CSV file, separated by the | 
		getline(stream, smallData, '|');
		int year = 0;
		if (testNum == 0) {
			string actualYear = "";
			for (int i = 3; i < 7; i++) {
				actualYear += smallData[i];
			}
			year += stoi(actualYear);
		}
		else {
			year += stoi(smallData);
		}
		getline(stream, smallData, '|');
		string state = smallData;
		getline(stream, smallData, '|');
		string industry = smallData;
		getline(stream, smallData, '|');
		float value = stof(smallData);
		// creating a GDP Node from the data above and pushing it back into the GDP Table
		table.push_back(GDP(year, industry, state, value));
		testNum++;
		if (testNum > 200000) {
			return;
		}
	}
}

// Merge Sort Function ()
void mergeSort3WayTable(vector<GDP>& table) {
	if (table.size() <= 1) {
		return;
	}

	// Splitting the table vector into 3 parts
	int midIndex1 = (table.size() - 1) / 3;
	int midIndex2 = 2 * ((table.size() - 1) / 3) + 1;
	vector<GDP> array1;
	vector<GDP> array2;
	vector<GDP> array3;

	for (int i = 0; i < midIndex1; ++i) {
		array1.push_back(table[i]);
	}
	for (int i = midIndex1; i < midIndex2; ++i) {
		array2.push_back(table[i]);
	}
	for (int i = midIndex2; i < table.size(); ++i) {
		array3.push_back(table[i]);
	}
	mergeSort3WayTable(array1);
	mergeSort3WayTable(array2);
	mergeSort3WayTable(array3);
	mergeVector(array1, array2, array3, table);
}
void mergeVector(vector<GDP>& arr1, vector<GDP>& arr2, vector<GDP>& arr3, vector<GDP>& table) {
	int arr1Size = arr1.size(), arr2Size = arr2.size(), arr3Size = arr3.size();
	int arr1Index = 0, arr2Index = 0, arr3Index = 0, mergedIndex = 0;
	// while all 3 of the sections still have values
	while ((arr1Index < arr1Size) && (arr2Index < arr2Size) && (arr3Index < arr3Size)) {
		if (arr1[arr1Index] > arr2[arr2Index]) {
			if (arr1[arr1Index] > arr3[arr3Index]) {
				table[mergedIndex++] = arr1[arr1Index++];
			}
			else {
				table[mergedIndex++] = arr3[arr3Index++];
			}
		}
		else {
			if (arr2[arr2Index] > arr3[arr3Index]) {
				table[mergedIndex++] = arr2[arr2Index++];
			}
			else {
				table[mergedIndex++] = arr3[arr3Index++];
			}
		}
	}
	// If the first section and second section of the 3 way still has values, check those two first 
	while ((arr1Index < arr1Size) && (arr2Index < arr2Size)) {
		if (arr1[arr1Index] > arr2[arr2Index]) {
			table[mergedIndex++] = arr1[arr1Index++];
		}
		else {
			table[mergedIndex++] = arr2[arr2Index++];
		}
	}
	// Then, if the 2nd and 3rd section still has values, check them 
	while ((arr2Index < arr2Size) && (arr3Index < arr3Size)) {
		if (arr2[arr2Index] > arr3[arr3Index]) {
			table[mergedIndex++] = arr2[arr2Index++];
		}
		else {
			table[mergedIndex++] = arr3[arr3Index++];
		}
	}
	// Then if the 3rd and 1st section still has values, check them
	while ((arr3Index < arr3Size) && (arr1Index < arr1Size)) {
		if (arr1[arr1Index] > arr3[arr3Index]) {
			table[mergedIndex++] = arr1[arr1Index++];
		}
		else {
			table[mergedIndex++] = arr3[arr3Index++];
		}
	}
	// Add in whatever is left
	while (arr1Index < arr1Size) {
		table[mergedIndex++] = arr1[arr1Index++];
	}
	while (arr2Index < arr2Size) {
		table[mergedIndex++] = arr2[arr2Index++];
	}
	while (arr3Index < arr3Size) {
		table[mergedIndex++] = arr3[arr3Index++];
	}
}

// Quick Sort Function ()
void swapGDP(GDP& node1, GDP& node2) {
	GDP temp = node1;
	node1 = node2;
	node2 = temp;
}
void quickSortTable(vector<GDP>& table) {
	stack<pair<int, int>> indexStack;
	int start = 0;
	int end = table.size() - 1;
	indexStack.push({ start, end });

	while (!indexStack.empty()) {
		start = indexStack.top().first;
		end = indexStack.top().second;
		indexStack.pop();

		int pivotIndex = partitionTable(table, start, end);

		if (pivotIndex - 1 > start) {
			indexStack.push({ start, pivotIndex - 1 });
		}

		if (pivotIndex + 1 < end ) {
			indexStack.push({ pivotIndex + 1, end });
		}
	}
}
int partitionTable(vector<GDP>& table, int start, int end) {
	int pivotIndex = start;
	GDP pivot = table[pivotIndex];
	int upPtr = start + 1; int downPtr = end;

	if ((end - start) + 1 <= 2) {
		if (table[end] > pivot) {
			swap(table[pivotIndex], table[downPtr]);
			return downPtr;
		}
		else
			return pivotIndex;
	}

	while (upPtr < downPtr) {
		while (upPtr < end) {
			if (table[upPtr] < pivot) {
				break;
			}
			upPtr++;
		}
		while (downPtr > start) {
			if (table[downPtr] > pivot) {
				break;
			}
			downPtr--;
		}
		if (upPtr < downPtr) {
			swapGDP(table[upPtr], table[downPtr]);
		}
	}
	swapGDP(table[pivotIndex], table[downPtr]);
	return downPtr;
} 

// Functions for Main ()
void findGDP(vector<GDP>& table, vector<GDP>& found, string state, int year) {
	// iterating through each GDP data node in table, and if all the conditions matches with the user specified ones, add onto the found array
	for (int i = 0; i < table.size(); ++i) {
		if ((table[i].getState() == state) && (table[i].getYear() == year) && (table[i].getValue() > 0)) {
			found.push_back(table[i]);
		}
	}
}
void sectionGDP(vector<GDP>& table, vector<GDP>& result, int x, int option, bool uniqueName, bool uniqueState, bool uniqueYear, bool selectState, string stateName, int startYear, int endYear) {
	set<string> name;
	set<string> state;
	set<int> year;
	// iterating through the sorted table array and adding a desired range of GDP data nodes to the result array
	int count = 0;
	if (option == 1) {
		for (int i = table.size() - 1; i >= 0; --i) {
			// checking for different uniqueness options depending on what the user wants
			if (((name.find(table[i].getName()) == name.end()) || (!uniqueName)) && ((state.find(table[i].getState()) == state.end()) || (!uniqueState)) && (table[i].getValue() > 0) && ((year.find(table[i].getYear()) == year.end()) || !uniqueYear) && ((table[i].getState() == stateName) || (!selectState)) && ((table[i].getYear() >= startYear) && (table[i].getYear() <= endYear))) {
				result.push_back(table[i]);
				name.insert(table[i].getName());
				state.insert(table[i].getState());
				year.insert(table[i].getYear());
				count++;
			}
			if (count >= x) {
				break;
			}
		}
	}
	else {
		for (int i = 0; i < table.size(); ++i) {
			// checking for different uniqueness options depending on what the user wants
			if (((name.find(table[i].getName()) == name.end()) || (!uniqueName)) && ((state.find(table[i].getState()) == state.end()) || (!uniqueState)) && (table[i].getValue() > 0) && ((year.find(table[i].getYear()) == year.end()) || !uniqueYear) && ((table[i].getState() == stateName) || (!selectState)) && ((table[i].getYear() >= startYear) && (table[i].getYear() <= endYear))) {
				result.push_back(table[i]);
				name.insert(table[i].getName());
				state.insert(table[i].getState());
				year.insert(table[i].getYear());
				count++;
			}
			if (count >= x) {
				break;
			}
		}
	}
}
void printGDPStateYear(vector<GDP>& table) {
	// Iterating through the found vector array and printing each value
	for (int i = 0; i < table.size(); i++) {
		GDP current = table[i];
		cout << "	" << current.getName() << " | GDP: $" ;
		// printing a rounded flat value so it's representative of money
		printf("%.2f", current.getValue());
		cout << endl;
	}
}
void printMenu() {
	cout << "=======================================================================================================================" << endl;
	cout << "				Program Menu" << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "	1. Compare 3-Way Merge vs Iterative Quick Sort." << endl;
	cout << "	2. Find the GDP information of a specified US state during a given year" << endl;
	cout << "	3. Find the ____ most productive industries in the US within a given time period." << endl;
	cout << "	4. Find the ____ least productive industries in the US within a given time period." << endl;
	cout << "	5. Find number 3, but within a specified US state." << endl;
	cout << "	6. Find number 4, but within a specified US state." << endl;
	cout << "	0. Exit Program" << endl;
	cout << "=======================================================================================================================" << endl;
	cout << endl;
}
void printGDPNode(GDP current) {
	cout << "Year: " << current.getYear() << " | State: " << current.getState() << " | " << current.getName() << " | GDP: $";
	printf("%.2f", current.getValue());
	cout << endl;
}

// Main Function ()
int main() {
	// Initializing big GDP Table
	cout << "Attempting to Connect to Database....." << endl;
	vector<GDP> GDPTable;
	// copying GDPTable for comparison purposes
	readCSV("Actual_GDP_Data.csv", GDPTable);
	cout << "Connection Successfully Established!" << endl << endl;
	// Displaying the options to the user
	int userInput = -1;
	cout << "Welcome to the Alrice GDP Data Viewer. Here are your options for this program:" << endl;
	bool menu = true;
	bool sorted = false;
	while (userInput != 0) {
		// Displaying the Menu
		if (menu) {
			printMenu();
			menu = false;
		}
		cout << "Please enter the number associated with the operation you would like to perform: ";
		cin >> userInput;
		if ((userInput > 2) && (!sorted)) {
			cout << "Please wait a moment...." << endl;
			auto sortStart = high_resolution_clock::now();
			quickSortTable(GDPTable);
			auto sortEnd = high_resolution_clock::now();
			auto sortDuration = duration_cast<seconds>(sortEnd - sortStart);
			cout << "Sorting completed in " << sortDuration.count() << " seconds." << endl;
			sorted = true;
		}
		if (userInput == 1) {
			cout << "Computing......" << endl;
			vector<GDP> GDPTable2;
			vector<GDP> GDPTable3;
			readCSV("Actual_GDP_Data.csv", GDPTable2);
			readCSV("Actual_GDP_Data.csv", GDPTable3);
			// Calling the merge functions and timing them with <chrono>
			auto mergeStart = high_resolution_clock::now();
			mergeSort3WayTable(GDPTable3);
			auto mergeEnd = high_resolution_clock::now();
			auto mergeDuration = duration_cast<seconds>(mergeEnd - mergeStart);
			auto quickStart = high_resolution_clock::now();
			quickSortTable(GDPTable2);
			auto quickEnd = high_resolution_clock::now();
			auto quickDuration = duration_cast<seconds>(quickEnd - quickStart);
			cout << "Done." << endl;
			cout << "***********************************************************************************************************************" << endl;
			cout << "3-Way Merge Sort of " << GDPTable2.size() << " GDP Data tuples was completed in: " << mergeDuration.count() << " seconds." << endl;
			cout << "Iterative Quick Sort of " << GDPTable2.size() << " GDP Data tuples was completed in: " << quickDuration.count() << " seconds." << endl;
			cout << "***********************************************************************************************************************" << endl;
			cout << endl;
		}
		else if (userInput == 2) {
			cout << "What state would you like to see GDP Data for? Please enter the 2 letter initials of the state you want: ";
			string state;
			cin >> state;
			cout << "What year would you like to see GDP Data from? ";
			int year;
			cin >> year;
			vector<GDP> found;
			cout << endl;
			// calling the finder function to give the vector array of GDP data with the specified conditions
			findGDP(GDPTable, found, state, year);
			// if no data was found with those conditions, print out accordingly
			if (found.size() == 0) {
				cout << "Unfortunately, no GDP Data could be found for the criterias that you have selected" << endl;
			}
			else {
				cout << "In " << state << " during " << year << ", below are the different tracked industries and their GDPs for this year:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				printGDPStateYear(found);
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				cout << endl;
			}
		}
		else if ((userInput > 2) && (userInput < 7)) {
			if ((userInput == 3) || (userInput == 5)) {
				cout << "How many x values do you want to include in this Top X Ranking? ";
			}
			else {
				cout << "How many x values do you want to include in this Bottom X Ranking? ";
			}
			int x;
			cin >> x;
			// checking if the user input a range too low
			if (x < 1) {
				cout << "Input too low, changing to 1..." << endl;
				x = 1;
			}
			// The user only get unique options for state if they didn't select option 5 or 6
			cout << "Do you want the Industries to be unique? 1(Y) | 0(N): ";
			bool uniqueName;
			cin >> uniqueName;
			bool uniqueState = 0;
			if (userInput < 5) {
				cout << "Do you want the State to be unique? 1(Y) | 0(N): ";
				cin >> uniqueState;
			}
			bool uniqueYear = 0;
			bool selectYear;
			cout << "Do you want to specify time range? 1(Y) | 0(N): ";
			cin >> selectYear;
			int startYear = 0; int endYear = 3000;
			if (selectYear) {
				cout << "Please enter the desired start year: " << endl;
				cin >> startYear;
				cout << "Please enter the desired end year: " << endl;
				cin >> endYear;
			}
			// The user only gets the option for unique years if the time range given is more than x years long
			if (endYear - startYear > x + 1) {
				cout << "Do you want the Year to be unique? 1(Y) | 0(N): ";
				cin >> uniqueYear;
			}
			bool selectState = 0;
			// The user only gets the option to specify the state if they selected No for UniqueStates and if they selected 5 or 6
			if (!uniqueState) {
				// if the user selected option 5, select state is automatically chosen
				if ((userInput == 5) || (userInput == 6)) {
					selectState = 1;
				}

			}
			else {
				selectState = 0;
			}
			string stateName;
			if (selectState) {
				cout << "What state would you like to restrict the data to? Please enter the 2 letter initials: ";
				cin >> stateName;
				if (stateName.size() > 2) {
					selectState = false;
				}
			}
			else {
				stateName = "ZZ";
			}
			cout << "Calculating...." << endl;

			vector<GDP> section;
			// Grabbing desired top x of the sorted GDPTable
			if ((userInput == 3) || (userInput == 5)) {
				sectionGDP(GDPTable, section, x, 0, uniqueName, uniqueState, uniqueYear, selectState, stateName, startYear, endYear);
			}
			else {
				sectionGDP(GDPTable, section, x, 1, uniqueName, uniqueState, uniqueYear, selectState, stateName, startYear, endYear);
			}
			// Printing values from the section subarray
			cout << "***********************************************************************************************************************" << endl;
			cout << "				";
			if ((userInput == 3) || (userInput == 5)) {
				cout << "Top " << x << " Industries Production" << endl;
			}
			else {
				cout << "Bottom " << x << " Industries Production" << endl;
			}
			cout << "***********************************************************************************************************************" << endl;
			for (int i = 0; i < section.size(); i++) {
				cout << i + 1 << ". ";
				printGDPNode(section[i]);
			}
			cout << endl;
		}
		else if (userInput == 0) {
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << "Awwh...Sad to see you go :(" << endl;
			cout << "Have a good day!" << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
		}
		// Giving the user the option to choose whether they want to see the menu again after executing their desired operations unless they chose 0, which ends the program
		if (userInput != 0) {
			cout << "Would you like to see the options menu again? 1(Y) or 0(N): ";
			int chosenMenu;
			cin >> chosenMenu;
			if (chosenMenu == 1) {
				menu = true;
			}
		}
	}

	return 0;
}