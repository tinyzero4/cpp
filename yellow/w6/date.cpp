#include "date.h"
#include <iomanip>

Date::Date(int new_year, int new_month, int new_day) {
    year = new_year;
    if (new_month > 12 || new_month < 1) {
        throw logic_error("Month value is invalid: " + to_string(new_month));
    }
    month = new_month;
    if (new_day > 31 || new_day < 1) {
        throw logic_error("Day value is invalid: " + to_string(new_day));
    }
    day = new_day;
}

int Date::GetYear() const {
    return year;
}

int Date::GetMonth() const {
    return month;
}

int Date::GetDay() const {
    return day;
}

Date ParseDate(istream &is) {
    bool ok = true;

    int year;
    ok = ok && (is >> year);
    ok = ok && (is.peek() == '-');
    is.ignore(1);

    int month;
    ok = ok && (is >> month);
    ok = ok && (is.peek() == '-');
    is.ignore(1);

    int day;
    ok = ok && (is >> day);
//    ok = ok && is.eof();

    if (!ok) {
        throw logic_error("Wrong date format");
    }
    return {year, month, day};
}

ostream &operator<<(ostream &stream, const Date &date) {
    stream << setw(4) << setfill('0') << date.GetYear() << "-"
           << setw(2) << setfill('0') << date.GetMonth() << "-"
           << setw(2) << setfill('0') << date.GetDay();
    return stream;
}

bool operator==(const Date &l, const Date &r) {
    return l.GetYear() == r.GetYear() && l.GetMonth() == r.GetMonth() && l.GetDay() == r.GetDay();
}

bool operator>(const Date &l, const Date &r) {
    if (l.GetYear() > r.GetYear()) return true;
    if (l.GetYear() == r.GetYear() && l.GetMonth() > r.GetMonth()) return true;
    if (l.GetYear() == r.GetYear() && l.GetMonth() == r.GetMonth() && l.GetDay() > r.GetDay()) return true;
    return false;
}

bool operator<(const Date &l, const Date &r) {
    if (l.GetYear() < r.GetYear()) return true;
    if (l.GetYear() == r.GetYear() && l.GetMonth() < r.GetMonth()) return true;
    if (l.GetYear() == r.GetYear() && l.GetMonth() == r.GetMonth() && l.GetDay() < r.GetDay()) return true;
    return false;
}

bool operator>=(const Date &l, const Date &r) {
    return l > r || l == r;
}

bool operator<=(const Date &l, const Date &r) {
    return l < r || l == r;
}

bool operator!=(const Date &l, const Date &r) {
    return !(l == r);
}