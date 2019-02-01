#include "test_runner.h"

#include <functional>
#include <optional>

using namespace std;

template<typename T>
class LazyValue {
public:
    explicit LazyValue(const std::function<T()> init): init_(std::move(init)) {}

    bool HasValue() const {
        return value.has_value();
    }

    const T& Get() const {
        if (!value) value = init_();
        return *value;
    }

private:
    mutable std::optional<T> value;
    std::function<T()> init_;
};

void UseExample() {
    const string big_string = "Giant amounts of memory";

    LazyValue<string> lazy_string([&big_string] { return big_string; });

    ASSERT(!lazy_string.HasValue());
    ASSERT_EQUAL(lazy_string.Get(), big_string);
    ASSERT_EQUAL(lazy_string.Get(), big_string);
}

void TestInitializerIsntCalled() {
    bool called = false;

    {
        LazyValue<int> lazy_int([&called] {
            called = true;
            return 0;
        });
    }
    ASSERT(!called);
}

void DoSomething(const vector<string>& v) {
    for (auto it = v.begin(); it != v.end(); ++it) {
        const std::string &s = *it;
        cout << s;
    }

    string string1;
    auto l = [string1]() {
        string a = string1;
    };
}


class MyClass {
    void Foo() {
        MyClass* p = this;
        cout << p;
    }

    void Bar() const {
        const MyClass* const p = this;
        cout << p;
    }
};

int main() {
    TestRunner tr;
    RUN_TEST(tr, UseExample);
    RUN_TEST(tr, TestInitializerIsntCalled);
    return 0;
}