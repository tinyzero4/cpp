#include "airline_ticket.h"
#include "test_runner.h"
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <tuple>
using namespace std;

#define SORT_BY(field) [](const AirlineTicket& lhs, const AirlineTicket& rhs) { \
    return lhs.field < rhs.field;    \
}

bool operator==(const Date &l, const Date &r) {
    return tie(l.year, l.month, l.day) == tie(r.year, r.month, r.day);
}

bool operator<(const Date &l, const Date &r) {
    return tie(l.year, l.month, l.day) < tie(r.year, r.month, r.day);
}

ostream &operator<<(ostream &stream, const Date &date) {
    stream << setw(4) << setfill('0') << date.year << "-"
           << setw(2) << setfill('0') << date.month << "-"
           << setw(2) << setfill('0') << date.day;
    return stream;
}

bool operator==(const Time &l, const Time &r) {
    return tie(l.hours, l.minutes) == tie(r.hours, r.minutes);
}

bool operator<(const Time &l, const Time &r) {
    return tie(l.hours, l.minutes) < tie(r.hours, r.minutes);
}

ostream &operator<<(ostream &stream, const Time &time) {
    stream << time.hours << "-" << time.minutes;
    return stream;
}

void TestSortBy() {
    vector<AirlineTicket> tixs = {
            {"VKO", "AER", "Utair",     {2018, 2, 28}, {17, 40}, {2018, 2, 28}, {20,  0}, 1200},
            {"AER", "VKO", "Utair",     {2018, 3,  5}, {14, 15}, {2018, 3,  5}, {16, 30}, 1700},
            {"AER", "SVO", "Aeroflot",  {2018, 3,  5}, {18, 30}, {2018, 3,  5}, {20, 30}, 2300},
            {"PMI", "DME", "Iberia",    {2018, 2,  8}, {23, 00}, {2018, 2,  9}, { 3, 30}, 9000},
            {"CDG", "SVO", "AirFrance", {2018, 3,  1}, {13, 00}, {2018, 3,  1}, {17, 30}, 8000},
    };

    sort(begin(tixs), end(tixs), SORT_BY(price));
    ASSERT_EQUAL(tixs.front().price, 1200);
    ASSERT_EQUAL(tixs.back().price, 9000);

    sort(begin(tixs), end(tixs), SORT_BY(from));
    ASSERT_EQUAL(tixs.front().from, "AER");
    ASSERT_EQUAL(tixs.back().from, "VKO");

    sort(begin(tixs), end(tixs), SORT_BY(arrival_date));
    ASSERT_EQUAL(tixs.front().arrival_date, (Date{2018, 2, 9}));
    ASSERT_EQUAL(tixs.back().arrival_date, (Date{2018, 3, 5}));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSortBy);
}