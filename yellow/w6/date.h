#pragma once

#include "string"
#include "stdexcept"

using namespace std;

class Date {
public:
    Date(int new_year, int new_month, int new_day);

    int GetYear() const;

    int GetMonth() const;

    int GetDay() const;

private:
    int year;
    int month;
    int day;
};

Date ParseDate(istream &is);

ostream &operator<<(ostream &stream, const Date &date);

bool operator==(const Date &l, const Date &r);

bool operator>(const Date &l, const Date &r);

bool operator<(const Date &l, const Date &r);

bool operator>=(const Date &l, const Date &r);

bool operator<=(const Date &l, const Date &r);

bool operator!=(const Date &l, const Date &r);