#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

enum Occupations {
    STUDENT,
    TEACHER,
    POLICEMEN
};

static map<Occupations, string> occupationsLabel = {
        {Occupations::STUDENT,   "Student"},
        {Occupations::TEACHER,   "Teacher"},
        {Occupations::POLICEMEN, "Policeman"},
};

class Human {
public:
    Human(string name, const Occupations &occupation) : _name(std::move(name)), _occupation(occupation) {}

    string Name() const {
        return _name;
    }

    virtual void Walk(const string &destination) const {
        perform("walks to", destination);
    }

    string Occupation() const {
        return occupationsLabel[_occupation];
    }

protected:

    void perform(const string &action, const string &onSubject = "") const {
        cout << Occupation() << ": " << Name() << " " << action;
        if (!onSubject.empty()) cout << ": " << onSubject;
        cout << endl;
    }

private:
    const string _name;
    const Occupations _occupation;
};


class Student : public Human {
public:

    Student(const string &name, string favouriteSong) :
            Human(name, Occupations::STUDENT), _favouriteSong(std::move(favouriteSong)) {}

    void Learn() {
        Human::perform("learns");
    }

    void Walk(const string &destination) const override {
        Human::Walk(destination);
        SingSong();
    }

    void SingSong() const {
        Human::perform("sings a song", _favouriteSong);
    }

private:
    const string _favouriteSong;
};


class Teacher : public Human {
public:
    Teacher(const string &name, string subject) : Human(name, Occupations::TEACHER), _subject(std::move(subject)) {}

    void Teach() {
        perform("teaches", _subject);
    }

public:
    const string _subject;
};


class Policeman : public Human {
public:
    explicit Policeman(const string &name) : Human(name, Occupations::POLICEMEN) {}

    void Check(Human &human) const {
        stringstream ss;
        ss << "checks " << human.Occupation() << ". " << human.Occupation() << "'s name is";
        perform(ss.str(), human.Name());
    }
};


void VisitPlaces(const Human &human, const vector<string> &places) {
    for (string const &p : places) {
        human.Walk(p);
    }
}

int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    return 0;
}
