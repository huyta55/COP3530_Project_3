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
		return (value < compare.value);
	}
	bool operator > (const GDP& compare) {
		return (value > compare.value);
	}
};
