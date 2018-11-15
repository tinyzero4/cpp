#include "database_test.h"
#include "condition_parser.h"
#include "test_runner.h"
#include <vector>

#include <sstream>

using namespace std;

void AssertDbPrintOutput(const Database &db, const string &hint, const vector<string> &expected) {
    stringstream str;
    db.Print(str);

    string str_print;

    size_t i = 0;
    while (getline(str, str_print)) {
        stringstream msg;
        msg << hint << ' ' << i;
        AssertEqual(str_print, expected[i], msg.str());
        i++;
    }
    stringstream eof_msg;
    eof_msg << hint << " eof";
    Assert(str.eof(), eof_msg.str());
}

void TestDatabasePrint() {
    {
        Database db;
        db.Add({2017, 1, 1}, "Holiday");
        db.Add({2017, 1, 1}, "Holiday");
        db.Add({2017, 1, 1}, "Holiday");
        db.Add({2017, 3, 8}, "Holiday");
        db.Add({2017, 3, 8}, "Holiday");
        db.Add({2017, 3, 8}, "Holiday");
        db.Add({2017, 3, 8}, "Holiday");
        db.Add({2017, 3, 8}, "Holiday");
        db.Add({2017, 3, 8}, "Holiday");
        db.Add({2017, 1, 1}, "New Year");
        db.Add({2017, 1, 1}, "New Year");
        db.Add({2017, 3, 8}, "Holiday");
        db.Add({2017, 3, 8}, "Holiday");
        db.Add({2017, 3, 8}, "Holiday");
        db.Add({2017, 1, 1}, "Holiday");
        db.Add({2017, 1, 1}, "Holiday");
        db.Add({2017, 1, 1}, "Holiday");

        AssertDbPrintOutput(db, "print value", {
                "2017-01-01 Holiday",
                "2017-01-01 New Year",
                "2017-03-08 Holiday",
                ""
        });
    }
    {
        Database db;
        stringstream str;
        db.Print(str);

        AssertDbPrintOutput(db, "print value", {
                ""
        });
    }
}

void TestDatabaseDel() {
    {
        Database db;
        int removed = 0;

        db.Add({2017, 6, 1}, "1st of June");
        db.Add({2017, 6, 2}, "2nd of June");
        db.Add({2017, 6, 3}, "3rd of June");
        db.Add({2017, 6, 4}, "4th of June");
        db.Add({2017, 6, 5}, "5th of June");
        db.Add({2017, 7, 8}, "8th of July");
        db.Add({2017, 7, 8}, "Someone's birthday");
        db.Add({2017, 7, 8}, "Buy groceries");
        db.Add({2017, 7, 9}, "9th of July");
        db.Add({2017, 7, 10}, "10th of July");
        db.Add({2017, 7, 11}, "11th of July");
        db.Add({2017, 7, 12}, "12th of July");
        db.Add({2017, 7, 13}, "13th of July");
        db.Add({2017, 7, 14}, "14th of July");


        removed = db.RemoveIf([](const Date &date, const string &event) { return date == Date(2017, 7, 8); });
        AssertEqual(removed, 3, "delete case 1");
//        removed = db.RemoveIf([](const Date &date, const string &event) { return event > "10th of July"; });
//        AssertEqual(removed, 10, "delete case 10");


        AssertDbPrintOutput(db, "delete case 1 print", {
                "2017-06-01 1st of June",
                "2017-06-02 2nd of June",
                "2017-06-03 3rd of June",
                "2017-06-04 4th of June",
                "2017-06-05 5th of June",
                "2017-07-09 9th of July",
                "2017-07-10 10th of July",
                "2017-07-11 11th of July",
                "2017-07-12 12th of July",
                "2017-07-13 13th of July",
                "2017-07-14 14th of July",
                ""
        });
    }
    {
        Database db;

        db.Add({2017, 6, 1}, "1st of June");
        db.Add({2017, 6, 2}, "2nd of June");
        db.Add({2017, 6, 3}, "3rd of June");
        db.Add({2017, 6, 4}, "4th of June");
        db.Add({2017, 6, 5}, "5th of June");
        db.Add({2017, 7, 8}, "8th of July");
        db.Add({2017, 7, 8}, "Someone's birthday");
        db.Add({2017, 7, 9}, "9th of July");
        db.Add({2017, 7, 10}, "10th of July");
        db.Add({2017, 7, 11}, "11th of July");
        db.Add({2017, 7, 12}, "12th of July");
        db.Add({2017, 7, 13}, "13th of July");
        db.Add({2017, 7, 14}, "14th of July");

        auto predicate = [](const Date &date, const string &event) {
            return date >= Date{2017, 7, 1};
        };

        int removed = db.RemoveIf(predicate);
        AssertEqual(removed, 8, "delete case 2");

        AssertDbPrintOutput(db, "delete case 2 print", {
                "2017-06-01 1st of June",
                "2017-06-02 2nd of June",
                "2017-06-03 3rd of June",
                "2017-06-04 4th of June",
                "2017-06-05 5th of June",
                ""
        });
    }
    {
        Database db;

        db.Add({2017, 6, 1}, "work");
        db.Add({2017, 6, 1}, "sleep");
        db.Add({2017, 6, 2}, "work");
        db.Add({2017, 6, 2}, "sleep");
        db.Add({2017, 6, 3}, "work");
        db.Add({2017, 6, 3}, "sleep");
        db.Add({2017, 6, 4}, "work");
        db.Add({2017, 6, 4}, "sleep");
        db.Add({2017, 6, 5}, "play computer games");
        db.Add({2017, 6, 5}, "sleep");
        db.Add({2017, 6, 6}, "visit parents");
        db.Add({2017, 6, 6}, "sleep");
        db.Add({2017, 6, 7}, "work");
        db.Add({2017, 6, 7}, "sleep");
        db.Add({2017, 6, 8}, "sleep");

        auto predicate = [](const Date &date, const string &event) {
            return event == "work";
        };

        int removed = db.RemoveIf(predicate);
        AssertEqual(removed, 5, "delete case 3");

        AssertDbPrintOutput(db, "delete case 3 print", {
                "2017-06-01 sleep",
                "2017-06-02 sleep",
                "2017-06-03 sleep",
                "2017-06-04 sleep",
                "2017-06-05 play computer games",
                "2017-06-05 sleep",
                "2017-06-06 visit parents",
                "2017-06-06 sleep",
                "2017-06-07 sleep",
                "2017-06-08 sleep",
                ""
        });
    }
}

void TestDatabaseLast() {
    {
        Database db;

        db.Add({2017, 11, 21}, "Tuesday");
        db.Add({2017, 11, 20}, "Monday");
        db.Add({2017, 11, 21}, "Weekly meeting");

        db.Last({2017, 11, 21});
        AssertEqual(db.Last({2017, 11, 20}), "2017-11-20 Monday", "last case 1");
        AssertEqual(db.Last({2017, 11, 21}), "2017-11-21 Weekly meeting", "last case 2");
        try {
            db.Last({2000, 1, 1});
            throw domain_error("last case 3: Shouldn't get here");
        } catch (const invalid_argument &e) {

        }

        auto predicate = [](const Date &date, const string &event) {
            return event == "Weekly meeting";
        };
        db.RemoveIf(predicate);
        AssertEqual(db.Last({2017, 11, 21}), "2017-11-21 Tuesday", "last case 4");
        AssertEqual(db.Last({9999, 12, 31}), "2017-11-21 Tuesday", "last case 5");

        db.Add({2017, 11, 21}, "Weekly meeting");

        AssertEqual(db.Last({2017, 11, 21}), "2017-11-21 Weekly meeting", "last case 6");
        AssertEqual(db.Last({9999, 12, 31}), "2017-11-21 Weekly meeting", "last case 7");

    }
}

void TestDatabaseFind() {
    {
        Database db;

        db.Add({2017, 6, 1}, "work");
        db.Add({2017, 6, 1}, "sleep");
        db.Add({2017, 6, 2}, "work");
        db.Add({2017, 6, 2}, "sleep");
        db.Add({2017, 6, 3}, "work");
        db.Add({2017, 6, 3}, "sleep");
        db.Add({2017, 6, 4}, "work");
        db.Add({2017, 6, 4}, "sleep");
        db.Add({2017, 6, 5}, "play computer games");
        db.Add({2017, 6, 5}, "sleep");
        db.Add({2017, 6, 6}, "visit parents");
        db.Add({2017, 6, 6}, "sleep");
        db.Add({2017, 6, 7}, "work");
        db.Add({2017, 6, 7}, "sleep");
        db.Add({2017, 6, 8}, "sleep");

        auto predicate1 = [](const Date &date, const string &event) {
            return event == "work";
        };

        vector<string> found1 = db.FindIf(predicate1);
        AssertEqual(found1, vector<string>{
                "2017-06-01 work",
                "2017-06-02 work",
                "2017-06-03 work",
                "2017-06-04 work",
                "2017-06-07 work"
        }, "find case 1");
    }
    {
        Database db;

        db.Add({2017, 6, 1}, "work");
        db.Add({2017, 6, 1}, "sleep");
        db.Add({2017, 6, 2}, "work");
        db.Add({2017, 6, 2}, "sleep");
        db.Add({2017, 6, 3}, "work");
        db.Add({2017, 6, 3}, "sleep");
        db.Add({2017, 6, 4}, "work");
        db.Add({2017, 6, 4}, "sleep");
        db.Add({2017, 6, 5}, "play computer games");
        db.Add({2017, 6, 5}, "sleep");
        db.Add({2017, 6, 6}, "visit parents");
        db.Add({2017, 6, 6}, "sleep");
        db.Add({2017, 6, 7}, "work");
        db.Add({2017, 6, 7}, "sleep");
        db.Add({2017, 6, 8}, "sleep");

        auto predicate1 = [](const Date &date, const string &event) {
            return event != "work";
        };

        vector<string> found1 = db.FindIf(predicate1);
        AssertEqual(found1, vector<string>{
                "2017-06-01 sleep",
                "2017-06-02 sleep",
                "2017-06-03 sleep",
                "2017-06-04 sleep",
                "2017-06-05 play computer games",
                "2017-06-05 sleep",
                "2017-06-06 visit parents",
                "2017-06-06 sleep",
                "2017-06-07 sleep",
                "2017-06-08 sleep"
        }, "find case 2");
    }
}

void TestDatabase() {
    TestDatabasePrint();
    TestDatabaseLast();
    TestDatabaseDel();
    TestDatabaseFind();
}