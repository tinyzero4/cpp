#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

struct Email {
    string from;
    string to;
    string body;
};

class Worker {
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        throw logic_error("Unimplemented");
    }

protected:
    void PassOn(unique_ptr<Email> email) const {
        if (next) next->Process(move(email));
    }
public:
    void SetNext(unique_ptr<Worker> next) {
        this->next = move(next);
    }

protected:
    unique_ptr<Worker> next;
};

class Reader : public Worker {
public:
    Reader(istream& stream): in_(stream) {};

    void Process(unique_ptr<Email> email) override {
        PassOn(move(email));
    }

    void Run() {
        for (;;) {
            auto email = make_unique<Email>();
            getline(in_, email->from);
            getline(in_, email->to);
            getline(in_, email->body);
            if (in_) {
                PassOn(move(email));
            } else {
                break;
            }
        }
    }
private:
    istream& in_;
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;

    Filter(Function predicate): predicate_(predicate) {}

public:
    void Process(unique_ptr<Email> email) override {
        if (predicate_(*email)) PassOn(move(email));
    }

private:
    Function predicate_;
};


class Copier : public Worker {
public:
    Copier(string cc): cc_(cc) {}

    void Process(unique_ptr<Email> email) override {
        if (email->to != cc_) {
            auto copy = make_unique<Email>(*email);
            copy->to = cc_;
            PassOn(move(email));
            PassOn(move(copy));
        } else {
            PassOn(move(email));
        }
    }
private:
    string cc_;
};


class Sender : public Worker {
public:
    Sender(ostream& out): out_(out) {}

    void Process(unique_ptr<Email> email) override {
        out_ << email->from << "\n" << email->to << "\n" << email->body << "\n";
        PassOn(move(email));
    }

private:
    ostream& out_;
};


// реализуйте класс
class PipelineBuilder {
public:
    explicit PipelineBuilder(istream& in): init_(make_unique<Reader>(in)) {
        worker_ = init_.get();
    }

    PipelineBuilder& FilterBy(Filter::Function filter) {
        unique_ptr<Filter> f(new Filter(move(filter)));
        Worker* w = f.get();
        worker_->SetNext(move(f));
        worker_ = w;
        return *this;
    }

    PipelineBuilder& CopyTo(string recipient) {
        unique_ptr<Copier> f(new Copier(move(recipient)));
        Worker* w = f.get();
        worker_->SetNext(move(f));
        worker_ = w;
        return *this;
    }

    PipelineBuilder& Send(ostream& out) {
        unique_ptr<Sender> f(new Sender(out));
        Worker* w = f.get();
        worker_->SetNext(move(f));
        worker_ = w;
        return *this;
    }

    unique_ptr<Worker> Build() {
        return move(init_);
    }
private:
    unique_ptr<Worker> init_;
    Worker* worker_;
};


void TestSanity() {
    string input = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "ralph@example.com\n"
            "erich@example.com\n"
            "I do not make mistakes of that kind\n"
    );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "erich@example.com\n"
            "richard@example.com\n"
            "Are you sure you pressed the right button?\n"
    );

    string res = outStream.str();
    ASSERT_EQUAL(expectedOutput, res);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}