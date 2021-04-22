#pragma once
#include <vector>
#include <string>

using namespace std;

class GDP {
	float value;
	string state;
	string industryName;
	int year;
public:
	// Default and overloaded constructors
	GDP() {
		value = 0;
		industryName = "Null";
		state = "Null";
		year = 0;
	}
	GDP(int year_, string name, string state_, float val) {
		year = year_;
		value = val;
		industryName = name;
		state = state_;
	}
	// setter functions for a GDP object
	void changeValue(float val) {
		value = val;
	}
	void changeIndustryName(string name) {
		industryName = name;
	}
	void changeYear(int year_) {
		year = year_;
	}
	void changeState(string name) {
		state = name;
	}
	// getter functions for a GDP object
	float getValue() {
		return value;
	}
	string getName() {
		return industryName;
	}
	int getYear() {
		return year;
	}
	string getState() {
		return state;
	}
	// overloading comparison operator
	bool operator < (const GDP& compare) {
		// comparing initial money values
		if (value < compare.value) {
			return true;
		}
		else if (value > compare.value) {
			return false;
		}
		else {
			// if the comparison is tied, compare the year since $5000 in 1989 is more than $5000 in 2002 because of inflation
			if (year > compare.year) {
				return true;
			}
			else if (year < compare.year) {
				return false;
			}
			else {
				// if even the year doesn't break the tie, go onto the first letter of state name
				if ((int)state[0] > (int) compare.state[0]) {
					return true;
				}
				else if ((int)state[0] < (int)compare.state[0]) {
					return false;
				}
				else {
					// if even the above doesn't break the tie, go onto first letter of industry name
					if ((int)industryName[0] > (int)compare.industryName[0]) {
						return true;
					}
					else if ((int)industryName[0] < (int)compare.industryName[0]) {
						return false;
					}
					else {
						// if still tie, go to string length of industry name
						if (industryName.size() > compare.industryName.size()) {
							return false;
						}
						else if (industryName.size() < compare.industryName.size()) {
							return true;
						}
						else {
							// else, just return true
							return true;
						}
					}
				}
			}
		}
	}
	bool operator > (const GDP& compare) {
		// comparing initial money values
		if (value < compare.value) {
			return false;
		}
		else if (value > compare.value) {
			return true;
		}
		else {
			// if the comparison is tied, compare the year since $5000 in 1989 is more than $5000 in 2002 because of inflation
			if (year > compare.year) {
				return false;
			}
			else if (year < compare.year) {
				return true;
			}
			else {
				// if even the year doesn't break the tie, go onto the first letter of state name
				if ((int)state[0] > (int) compare.state[0]) {
					return false;
				}
				else if ((int)state[0] < (int)compare.state[0]) {
					return true;
				}
				else {
					// if even the above doesn't break the tie, go onto first letter of industry name
					if ((int)industryName[0] > (int)compare.industryName[0]) {
						return false;
					}
					else if ((int)industryName[0] < (int)compare.industryName[0]) {
						return true;
					}
					else {
						// if still tie, go to string length of industry name
						if (industryName.size() > compare.industryName.size()) {
							return true;
						}
						else if (industryName.size() < compare.industryName.size()) {
							return false;
						}
						else {
							// else, just return false
							return false;
						}
					}
				}
			}
		}
	}
	// overloading other operators
	GDP operator=(GDP copy) {
		value = copy.getValue();
		state = copy.getState();
		industryName = copy.getName();
		year = copy.getYear();
		return *this;
	}
	bool operator== (GDP compare) {
		if ((value == compare.value) && (state == compare.state) && (industryName == compare.industryName) && (year == compare.year)) {
			return true;
		}
		return false;
	}
};
