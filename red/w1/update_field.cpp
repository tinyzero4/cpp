#include "airline_ticket.h"
#include "../w2/test_runner.h"
#include <iomanip>

using namespace std;

#define UPDATE_FIELD(ticket, field, values) {       \
    const auto field_it = values.find(#field);      \
    if (field_it != values.end()) {                 \
        istringstream is(field_it->second);         \
        is >> ticket.field;                         \
    }                                               \
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

ostream &operator<<(ostream &stream, const Time &time) {
    stream << time.hours << ":" << time.minutes;
    return stream;
}

bool operator==(const Time &l, const Time &r) {
    return tie(l.hours, l.minutes) == tie(r.hours, r.minutes);
}

bool operator<(const Time &l, const Time &r) {
    return tie(l.hours, l.minutes) < tie(r.hours, r.minutes);
}

istream &operator>>(istream &is, Date &d) {
    is >> d.year;
    is.ignore(1);
    is >> d.month;
    is.ignore(1);
    is >> d.day;
    return is;
}

istream &operator>>(istream &is, Time &t) {
    is >> t.hours;
    is.ignore(1);
    is >> t.minutes;
    return is;
}


void TestUpdate() {
    AirlineTicket t;
    t.price = 0;

    const map<string, string> updates1 = {
            {"departure_date", "2018-2-28"},
            {"departure_time", "17:40"},
    };
    UPDATE_FIELD(t, departure_date, updates1);
    UPDATE_FIELD(t, departure_time, updates1);
    UPDATE_FIELD(t, price, updates1);

    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 0);

    const map<string, string> updates2 = {
            {"price",        "12550"},
            {"arrival_time", "20:33"},
    };
    UPDATE_FIELD(t, departure_date, updates2);
    UPDATE_FIELD(t, departure_time, updates2);
    UPDATE_FIELD(t, arrival_time, updates2);
    UPDATE_FIELD(t, price, updates2);

    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 12550);
    ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestUpdate);
}