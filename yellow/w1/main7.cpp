#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template<class T>
ostream &operator<<(ostream &os, const vector<T> &s) {
    os << "{";
    bool first = true;
    for (const auto &x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template<class T>
ostream &operator<<(ostream &os, const set<T> &s) {
    os << "{";
    bool first = true;
    for (const auto &x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template<class K, class V>
ostream &operator<<(ostream &os, const map<K, V> &m) {
    os << "{";
    bool first = true;
    for (const auto &kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template<class T, class U>
void AssertEqual(const T &t, const U &u, const string &hint = {}) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string &hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
public:
    template<class TestFunc>
    void RunTest(TestFunc func, const string &test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        } catch (exception &e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        } catch (...) {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};

class Rational {
public:
    Rational() {
        p = 0;
        q = 1;
    }

    Rational(int numerator, int denominator) {
        if (numerator == 0) denominator = 1;
        int g = gcd(numerator < 0 ? numerator * -1 : numerator, denominator < 0 ? denominator * -1 : denominator);
        if (g < 0) g *= -1;

        if (denominator < 0) {
            denominator = -1 * denominator;
            numerator = -1 * numerator;
        }

        p = numerator / g;
        q = denominator / g;
    }

    int Numerator() const {
        return p;
    }

    int Denominator() const {
        return q;
    }

private:
    int p, q;

    int gcd(int a, int b) {
        if (a == 0)
            return b;
        return gcd(b % a, a);
    }
};

void TestDefaultConstructor() {
    Rational r;
    Assert(r.Numerator() == 0, "0");
    Assert(r.Denominator() == 1, "1");
}

void TestGcd() {
    Rational r(4, 6);
    Assert(r.Numerator() == 2, "2");
    Assert(r.Denominator() == 3, "3");
}

void TestNegative() {
    Rational r(5, -6);
    Assert(r.Numerator() == -5, "-5");
    Assert(r.Denominator() == 6, "6");
}

void TestPositive() {
    Rational r(-7, -8);
    Assert(r.Numerator() == 7, "7");
    Assert(r.Denominator() == 8, "8");
}

void TestZero() {
    Rational r(0, 2);
    Assert(r.Numerator() == 0, "0");
    Assert(r.Denominator() == 1, "1");
}

int main() {
    TestRunner runner;
    runner.RunTest(TestDefaultConstructor, "TestDefaultConstructor");
    runner.RunTest(TestGcd, "TestGcd");
    runner.RunTest(TestNegative, "TestNegative");
    runner.RunTest(TestPositive, "TestPositive");
    runner.RunTest(TestZero, "TestZero");

    return 0;
}