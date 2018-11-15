#include "date_test.h"

void TestParseDate() {
    {
        istringstream is("2017-11-18");
        Date date = ParseDate(is);
        AssertEqual(date, Date{2017, 11, 18}, "Parse date 1");
        AssertEqual(date.GetYear(), 2017, "Parse date 2");
        AssertEqual(date.GetMonth(), 11, "Parse date 3");
        AssertEqual(date.GetDay(), 18, "Parse date 4");
        Assert(date != Date{2017, 1, 1}, "Parse date 5");
    }
    {
        istringstream is("     2017-1-14");
        Date date = ParseDate(is);
        AssertEqual(date, Date{2017, 01, 14}, "Parse date 6");
        ostringstream os;
        os << date;
        AssertEqual(os.str(), "2017-01-14", "Parse date 7");
    }
    {
        istringstream is("0-1-1");
        Date date = ParseDate(is);
        AssertEqual(date, Date{0, 1, 1}, "Parse date 8");
        ostringstream os;
        os << date;
        AssertEqual(os.str(), "0000-01-01", "Parse date 9");
    }
    {
        istringstream is("9999-12-31");
        Date date = ParseDate(is);
        AssertEqual(date, Date{9999, 12, 31}, "Parse date 10");
        ostringstream os;
        os << date;
        AssertEqual(os.str(), "9999-12-31", "Parse date 11");
    }

}

void TestDateComparison() {
    {
        Assert(Date{2017, 11, 18} == Date{2017, 11, 18}, "Compare date 1");
        Assert(Date{2017, 11, 18} != Date{2017, 11, 19}, "Compare date 2");

        Assert(Date{2017, 11, 18} < Date{2017, 11, 19}, "Compare date 3");
        Assert(Date{2017, 10, 18} < Date{2017, 11, 19}, "Compare date 4");
        Assert(Date{2016, 11, 18} < Date{2017, 11, 19}, "Compare date 5");
        Assert(Date{2017, 11, 20} > Date{2017, 11, 19}, "Compare date 6");
        Assert(Date{2017, 12, 18} > Date{2017, 11, 19}, "Compare date 7");
        Assert(Date{2018, 11, 18} > Date{2017, 11, 19}, "Compare date 8");

        Assert(Date{2017, 11, 18} >= Date{2017, 11, 18}, "Compare date 9");
        Assert(Date{2017, 11, 18} <= Date{2017, 11, 18}, "Compare date 10");

        Assert(Date{2017, 11, 18} <= Date{2017, 11, 19}, "Compare date 11");
        Assert(Date{2017, 10, 18} <= Date{2017, 11, 19}, "Compare date 12");
        Assert(Date{2016, 11, 18} <= Date{2017, 11, 19}, "Compare date 13");
        Assert(Date{2017, 11, 20} >= Date{2017, 11, 19}, "Compare date 14");
        Assert(Date{2017, 12, 18} >= Date{2017, 11, 19}, "Compare date 15");
        Assert(Date{2018, 11, 18} >= Date{2017, 11, 19}, "Compare date 16");
    }
}

void TestDate() {
    TestParseDate();
    TestDateComparison();
}