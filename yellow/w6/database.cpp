#include "database.h"
#include "node.h"
#include <memory>
#include <stdexcept>
#include <algorithm>

void Database::Add(const Date &date, const string &event) {
    set<string> &events = records[date];
    if (events.count(event) == 0) {
        events.insert(event);
        records_ordered[date].push_back(event);
    }
}

int Database::RemoveIf(const function<bool(Date, string)> &f) {
    uint32_t count = 0;
    set<Date> affected_dates;
    for (auto &record : records) {
        auto &records_order = records_ordered[record.first];
        for (auto it = record.second.begin(); it != record.second.end();) {
            if (f(record.first, *it)) {
                records_order.erase(remove(records_order.begin(), records_order.end(), *it));
                record.second.erase(it++);
                count++;
                affected_dates.insert(record.first);

            }
            else ++it;
        }
    }

    for(auto it = records.begin();it != records.end();) {
        if (it->second.empty()) {
            auto date = it->first;
            records.erase(it++);
            records_ordered.erase(date);
        }
        else ++it;
    }

    return count;
}

vector<string> Database::FindIf(const function<bool(Date, string)> &f) const {
    vector<string> result;
    ostringstream entry;

    for (const auto &item : records_ordered) {
        for (const auto &e : item.second) {
            if (f(item.first, e)) {
                entry.clear();
                entry.str("");
                _PrintEvent(item.first, e, entry, false);
                result.push_back(entry.str());
            }
        }
    }
    return result;
}

void Database::_PrintEvent(const Date &date, const string &event, ostream &os, bool useNewline = true) const {
    os << date << " " << event;
    if (useNewline) os << endl;
}

void Database::Print(ostream &os) const {
    for (const auto &item : records_ordered) {
        for (const string &event : item.second) {
            _PrintEvent(item.first, event, os, true);
        }
    }
}

string Database::Last(const Date &date) const {
    if (records_ordered.empty()) throw invalid_argument("no entries");

    auto it = records_ordered.lower_bound(date);
    if (it == records_ordered.begin() && it->first != date) throw invalid_argument("no entries");

    const auto &closestDate = (it->first == date) ? *it : *(--it);
    vector<string> events = closestDate.second;
    if (events.empty()) throw invalid_argument("no entries");

    ostringstream entry;
    _PrintEvent(closestDate.first, *events.rbegin(), entry, false);

    return entry.str();
}