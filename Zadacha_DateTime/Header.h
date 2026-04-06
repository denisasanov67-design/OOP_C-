#pragma once

#include <iostream>
#include <string>
using namespace std;

class DateTime {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;

public:
	DateTime();
	DateTime(int y, int m, int d, int h, int min, int s);

	void input();
	bool Valid();
	bool Leap(int y) const;

	void Format1();
	void Format2();
	void Format3();

	int Week() const;

	long Days() const;

	int operator-(const DateTime& other) const;
	DateTime operator+(int days) const;
	DateTime operator-(int days) const;
	bool operator==(const DateTime& other) const;
	bool operator!=(const DateTime& other) const;
	bool operator<(const DateTime& other) const;
	bool operator>(const DateTime& other) const;
	int difference(DateTime other);

	static DateTime parselISO(string str);

	DateTime getEsterDate(int year);
};

