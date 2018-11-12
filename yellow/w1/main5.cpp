#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

int gcd(int a, int b) {
    if (a == 0)
        return b;
    return gcd(b % a, a);
}

class Rational {
public:
    Rational() {
        p = 0;
        q = 1;
    }

    Rational(int numerator, int denominator) {
        if (numerator == 0) denominator = 1;
        p = numerator;
        q = denominator;
        optimize();
    }

    int Numerator() const {
        return p;
    }

    void SetNumerator(int numerator) {
        p = numerator;
        optimize();
    }

    int Denominator() const {
        return q;
    }

    void SetDenominator(int denominator) {
        q = denominator;
        optimize();
    }

private:
    int p, q;

    void optimize() {
        int g = gcd(p, q);
        if (g < 0) g *= -1;

        bool pos = (p >= 0 && q >= 0) || (p < 0 && q < 0);
        if (!pos && q < 0) {
            q *= -1;
            p *= -1;
        }
        p /= g;
        q /= g;
    }
};

Rational operator+(const Rational &lhs, const Rational rhs) {
    return {
            lhs.Numerator() * rhs.Denominator() + rhs.Numerator() * lhs.Denominator(),
            lhs.Denominator() * rhs.Denominator()
    };
}

Rational operator-(const Rational &lhs, const Rational rhs) {
    return Rational(lhs.Numerator() * rhs.Denominator() - rhs.Numerator() * lhs.Denominator(),
                    lhs.Denominator() * rhs.Denominator());
}

bool operator==(const Rational &lhs, const Rational rhs) {
    return lhs.Denominator() == rhs.Denominator() && lhs.Numerator() == rhs.Numerator();
}

Rational operator*(const Rational &lhs, const Rational rhs) {
    return Rational(lhs.Numerator() * rhs.Numerator(),
                    lhs.Denominator() * rhs.Denominator());
}

Rational operator/(const Rational &lhs, const Rational rhs) {
    return Rational(lhs.Numerator() * rhs.Denominator(),
                    lhs.Denominator() * rhs.Numerator());
}

bool operator<(const Rational &lhs, const Rational &rhs) {
    return (lhs - rhs).Numerator() < 0;
}

ostream &operator<<(ostream &stream, const Rational &r) {
    stream << r.Numerator() << "/" << r.Denominator();
    return stream;
}

istream &operator>>(istream &stream, Rational &r) {
    if (stream) {
        int n = r.Numerator(), d = r.Denominator();
        stream >> n;
        stream.ignore(1);
        stream >> d;

        r.SetNumerator(n);
        r.SetDenominator(d);
    }
    return stream;
}

int main() {
    {
        const set<Rational> rs = {{1, 2},
                                  {1, 25},
                                  {3, 4},
                                  {3, 4},
                                  {1, 2}};
        if (rs.size() != 3) {
            cout << "Wrong amount of items in the set" << endl;
            return 1;
        }

        vector<Rational> v;
        for (auto x : rs) {
            v.push_back(x);
        }
        if (v != vector<Rational>{{1, 25},
                                  {1, 2},
                                  {3, 4}}) {
            cout << "Rationals comparison works incorrectly" << endl;
            return 2;
        }
    }

    {
        map<Rational, int> count;
        ++count[{1, 2}];
        ++count[{1, 2}];

        ++count[{2, 3}];

        if (count.size() != 2) {
            cout << "Wrong amount of items in the map" << endl;
            return 3;
        }
    }

    cout << "OK" << endl;
    return 0;
}