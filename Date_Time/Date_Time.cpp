#define _CRT_SECURE_NO_WARNINGS
#include "Header.h"
#include <cmath>
#include <iomanip>
#include <cstdio>
#include <sstream>
using namespace std;

DateTime::DateTime()
{
	year = 2000;
	month = 1;
	day = 1;
	hour = minute = second = 0;
}

DateTime::DateTime(int y, int m, int d, int h, int min, int s)
{
	year = y;
	month = m;
	day = d;
	hour = h;
	minute = min;
	second = s;
}

bool DateTime::Leap(int y) const
{
	return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

bool DateTime::Valid() {
	int daysMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	if (Leap(year)) daysMonth[1] = 29;

	if (year < 1) {
		throw DateTimeException("√од должен быть положительным числом");
	}

	if (month < 1 || month > 12) {
		throw DateTimeException("ћес€ц должен быть в диапазоне 1-12");
	}

	if (day < 1 || day > daysMonth[month - 1]) {
		throw DateTimeException("ƒень не соответствует мес€цу");
	}

	if (hour < 0 || hour > 23) {
		throw DateTimeException("„ас должен быть в диапазоне 0-23");
	}
		
	if (minute < 0 || minute > 59) {
		throw DateTimeException("ћинута должна быть в диапазоне 0-59");
	}

	if (second < 0 || second > 59) {
		throw DateTimeException("—екунда должна быть в диапазоне 0-59");
	}

	return true;
}
void DateTime::input() {
	string str;
	cout << "¬ведите дату (yyyy-MM-ddThh:mm:ss):";
	cin >> str;
	*this = parselISO(str);
}

void DateTime::inputMoonDate() 
{
	string str;
	cout << "¬ведите дату (дд.мм.гггг):";
	cin >> str;
	*this = parselISO(str);
}

int DateTime::getYear() const
{
	ostringstream oss;
	return year;
}

DateTime DateTime::parselISO(string str) {
	int day = stoi(str.substr(0, 2)); 
	int month = stoi(str.substr(3, 2));  
	int year = stoi(str.substr(6, 4)); 

	return DateTime(year, month, day, 0, 0, 0);
}

void DateTime::Format1() {
	cout << day << "." << month << "." << year << endl;
}

void DateTime::Format2() {
	string months[] = { "€нвар€","феврал€","марта","апрел€","ма€","июн€","июл€","августа","сент€бр€","окт€бр€","но€бр€","декабр€" };
	cout << day << " " << months[month - 1] << " " << year << endl;
}

void DateTime::Format3() {
	char old_fill = cout.fill('0');
	cout << setw(2) << day << "."
		<< setw(2) << month << "."
		<< setw(2) << year % 100 << endl;
	cout.fill(old_fill);
}

int DateTime::Week() const
{
	int a = (14 - month) / 12;
	int y = year - a;
	int m = month + 12 * a - 2;

	int d = (day + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12) % 7;

	return d;
}

long DateTime::Days() const
{
	long days = (year - 1) * 365 + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;

	int daysMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	if (Leap(year))
		daysMonth[1] = 29;

	for (int i = 0; i < month - 1; i++)
		days += daysMonth[i];

	days += day;

	return days;
}

int DateTime::operator-(const DateTime& other) const {
	return abs(Days() - other.Days());
}

DateTime DateTime::operator+(int days) const {
	DateTime result = *this;
	long totalDays = result.Days() + days;


	return result;
}

DateTime DateTime::operator-(int days) const {
	DateTime result = *this;
	long totalDays = result.Days() - days;
	return result;
}

bool DateTime::operator==(const DateTime& other) const {
	return (year == other.year &&
		month == other.month &&
		day == other.day &&
		hour == other.hour &&
		minute == other.minute &&
		second == other.second);
}

bool DateTime::operator!=(const DateTime& other) const {
	return !(*this == other);
}

bool DateTime::operator<(const DateTime& other) const {
	if (year != other.year) return year < other.year;
	if (month != other.month) return month < other.month;
	if (day != other.day) return day < other.day;
	if (hour != other.hour) return hour < other.hour;
	if (minute != other.minute) return minute < other.minute;
	return second < other.second;
}

bool DateTime::operator>(const DateTime& other) const {
	return other < *this;
}

int DateTime::difference(DateTime other)
{
	return abs(Days() - other.Days());
}

DateTime DateTime::getEsterDate(int Y)
{
	int a = Y % 4;
	int b = Y % 7;
	int c = Y % 19;

	int d = (19 * c + 15) % 30;
	int e = (2 * a + 4 * b - d + 34) % 7;

	int month = (d + e + 114) / 31;
	int day = ((d + e + 114) % 31) + 1;

	return DateTime(Y, month, day, 0, 0, 0);
}

string DateTime::formatTime(const string& hms) 
{
	return hms.substr(0, 2) + ":" + hms.substr(2, 2) + ":" + hms.substr(4, 2);
}