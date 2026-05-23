#define _CRT_SECURE_NO_WARNINGS

#include "Header.h"

#include <sstream>
#include <iomanip>

using namespace std;

// =====================
// DateTimeException
// =====================

DateTimeException::DateTimeException(const string& msg)
    : message(msg) {
}

const char* DateTimeException::what() const noexcept {
    return message.c_str();
}

// =====================
// DateTime
// =====================

DateTime::DateTime()
    : year(2000), month(1), day(1) {
}

DateTime::DateTime(int y, int m, int d)
    : year(y), month(m), day(d) {
}

bool DateTime::Leap(int y) const {

    return (y % 4 == 0 && y % 100 != 0) ||
        (y % 400 == 0);
}

bool DateTime::Valid() const {

    int daysMonth[] =
    {
        31,28,31,30,31,30,
        31,31,30,31,30,31
    };

    if (Leap(year))
        daysMonth[1] = 29;

    if (year < 1)
        throw DateTimeException("Ошибка: год должен быть положительным");

    if (month < 1 || month > 12)
        throw DateTimeException("Ошибка: месяц должен быть от 1 до 12");

    if (day < 1 || day > daysMonth[month - 1])
        throw DateTimeException("Ошибка: неверный день месяца");

    return true;
}

DateTime DateTime::parseISO(const string& str) {

    if (str.length() != 10 ||
        str[2] != '.' ||
        str[5] != '.') {

        throw DateTimeException(
            "Ошибка: формат даты должен быть дд.мм.гггг"
        );
    }

    int d = stoi(str.substr(0, 2));
    int m = stoi(str.substr(3, 2));
    int y = stoi(str.substr(6, 4));

    DateTime dt(y, m, d);

    dt.Valid();

    return dt;
}

string DateTime::getFileName() const {

    stringstream ss;

    ss << "Moon/moon"
        << setw(4) << setfill('0') << year
        << ".dat";

    return ss.str();
}

string DateTime::getFormattedDate() const {

    stringstream ss;

    ss << setw(2) << setfill('0') << day << "."
        << setw(2) << setfill('0') << month << "."
        << setw(4) << setfill('0') << year;

    return ss.str();
}

// =====================
// Форматирование времени
// =====================

string formatTime(const string& hms) {

    if (hms.size() < 6)
        return "Нет";

    return hms.substr(0, 2) + ":" +
        hms.substr(2, 2) + ":" +
        hms.substr(4, 2);
}