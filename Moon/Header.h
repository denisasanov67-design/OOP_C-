#pragma once

#include <iostream>
#include <string>
#include <exception>

using namespace std;

class DateTimeException : public exception {
private:
    string message;

public:
    DateTimeException(const string& msg);

    const char* what() const noexcept override;
};

class DateTime {
private:
    int year;
    int month;
    int day;

public:
    DateTime();
    DateTime(int y, int m, int d);

    bool Leap(int y) const;
    bool Valid() const;

    static DateTime parseISO(const string& str);

    string getFileName() const;
    string getFormattedDate() const;
};

string formatTime(const string& hms);