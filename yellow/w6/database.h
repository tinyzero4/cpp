#pragma once

#include <set>
#include <utility>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <functional>
#include "date.h"


using namespace std;

class Database {
public:
    void Add(const Date &date, const string &event);

    void Print(ostream &os) const;

    int RemoveIf(const function<bool(Date, string)> &f);

    vector<string> FindIf(const function<bool(Date, string)> &f) const;

    string Last(const Date &date) const;

private:
    map<Date, set<string>> records;
    map<Date, vector<string>> records_ordered;

    void _PrintEvent(const Date &date, const string &event, ostream &os, bool useNewline) const;
};

