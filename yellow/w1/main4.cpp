#include <iostream>
#include <sstream>

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

    int Denominator() const {
        return q;
    }

private:
    int p, q;

    void optimize() {
        int g = gcd(p < 0 ? -p : p, q < 0 ? -q : q);
        if (q < 0) {
            q = -q;
            p = -p;
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

ostream &operator<<(ostream &stream, const Rational &r) {
    stream << r.Numerator() << "/" << r.Denominator();
    return stream;
}

istream &operator>>(istream &stream, Rational &r) {
    if (stream.good()) {
        string def;
        stream >> def;
        if (def.empty()) return stream;

        int n, d;
        stringstream s(def);
        s >> n;
        s.ignore(1);
        s >> d;
        r = {n, d};
    } else {
        stream.setstate(ios::failbit);
    }
    return stream;
}


int main() {
    {
        ostringstream output;
        output << Rational(-6, 8);
        if (output.str() != "-3/4") {
            cout << "Rational(-6, 8) should be written as \"-3/4\"" << endl;
            return 1;
        }
    }

    {
        istringstream input("5/7");
        Rational r;
        input >> r;
        bool equal = r == Rational(5, 7);
        if (!equal) {
            cout << "5/7 is incorrectly read as " << r << endl;
            return 2;
        }
    }

    {
        istringstream input("5/7 10/8");
        Rational r1, r2;
        input >> r1 >> r2;
        bool correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Multiple values are read incorrectly: " << r1 << " " << r2 << endl;
            return 3;
        }

        input >> r1;
        input >> r2;
        correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Read from empty stream shouldn't change arguments: " << r1 << " " << r2 << endl;
            return 4;
        }
    }

    cout << "OK" << endl;
    return 0;
}