#include <iostream>
#include <sstream>
#include "vector"
#include "string"
#include "strstream"
#include "ios"
#include "iosfwd"
#include "iomanip"
#include "memory"
#include "math.h"

using namespace std;

const double PI = 3.14;
const string RECT = "RECT";
const string CIRCLE = "CIRCLE";
const string TRIANGLE = "TRIANGLE";


class Figure {
public:
    Figure(const string &name = "FIGURE") : name_(name) {}

    virtual string Name() const {
        return name_;
    }

    virtual double Perimeter() const = 0;

    virtual double Area() const = 0;

private:
    const string name_;

};

class Rect : public Figure {
public:
    Rect(int x, int y) : Figure(RECT), x_(x), y_(y) {}

    double Perimeter() const override {
        return 2 * x_ + 2 * y_;
    }

    double Area() const override {
        return x_ * y_;
    }

private:
    const int x_;
    const int y_;

};

class Circle : public Figure {
public:
    Circle(int r) : Figure(CIRCLE), r_(r) {}

    double Perimeter() const override {
        return 2 * PI * r_;
    }

    double Area() const override {
        return PI * r_ * r_;
    }

private:
    const int r_;
};

class Triangle : public Figure {
public:
    Triangle(int a, int b, int c) : Figure(TRIANGLE), a_(a), b_(b), c_(c) {}

    double Perimeter() const override {
        return a_ + b_ + c_;
    }

    double Area() const override {
        double p = Perimeter() / 2;
        return sqrt(p * (p - a_) * (p - b_) * (p - c_));
    }

private:
    const int a_;
    const int b_;
    const int c_;

};

shared_ptr<Figure> CreateFigure(istringstream &is) {
    string type;
    int a = 0, b = 0, c = 0;
    is >> type;
    if (type == RECT) {
        is >> a >> b;
        return make_shared<Rect>(a, b);
    } else if (type == CIRCLE) {
        is >> a;
        return make_shared<Circle>(a);
    } else {
        is >> a >> b >> c;
        return make_shared<Triangle>(a, b, c);
    }
}

int main() {
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line);) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "ADD") {
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto &current_figure : figures) {
                cout << fixed << setprecision(3)
                     << current_figure->Name() << " "
                     << current_figure->Perimeter() << " "
                     << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}