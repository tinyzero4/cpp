//#include "test_runner.h"

#include <vector>
#include <stdexcept>

using namespace std;


template<class T>
class Deque {
public:
    Deque() {
        front.resize(0);
        back.resize(0);
    }

    bool Empty() const {
        return front.empty() && back.empty();
    }

    uint64_t Size() const {
        return front.size() + back.size();
    }

    T &operator[](size_t index) {
        return At(index);
    }

    const T &operator[](size_t index) const {
        return static_cast<const T&>(this)->At(index);
    }

    T &At(size_t index) {
        if (Size() <= index) throw out_of_range("out of range");
        if (index < front.size()) return front.at(index);
        else return back.at(index - front.size());
    }

    T &At(size_t index) const {
        return static_cast<const T&>(this)->At();
    }

    T &Front() {
        if (Size() == 0) throw out_of_range("no items");
        if (!front.empty()) return front.back();
        else return back.at(0);
    }

    T &Back() {
        if (Size() == 0) throw out_of_range("no items");
        if (!back.empty()) return back.back();
        else return front.at(0);
    }

    T &Front() const {
        return static_cast<const T&>(this)->Front();
    }

    T &Back() const {
        return static_cast<const T&>(this)->Back();
    }

    void PushFront(const T &item) {
        front.push_back(item);
    }

    void PushBack(const T &item) {
        back.push_back(item);
    }


private:
    vector<T> front;
    vector<T> back;
};

//void TestOps() {
//    Deque<int> deque;
//
////    deque.PushBack(1);
//    deque.PushFront(2);
//
//    ASSERT_EQUAL(deque.Front(), 2);
//    ASSERT_EQUAL(deque.Back(), 2);
//    ASSERT_EQUAL(deque.Size(), 1);
//}
//
//
//int main() {
//    TestRunner runner;
//    runner.RunTest(TestOps, "Test Ops");
//    return 0;
//}