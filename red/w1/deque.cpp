#include <vector>
#include <stdexcept>

using namespace std;

template<class T>
class Deque {
private:
    vector<T> head;
    vector<T> tail;

public:
    Deque() {
        head.resize(0);
        tail.resize(0);
    }

    bool Empty() const {
        return head.empty() && tail.empty();
    }

    size_t Size() const {
        return head.size() + tail.size();
    }

    T &operator[](size_t i) {
        return i < head.size() ? head[head.size() - i - 1] : tail[i - head.size()];
    }

    const T &operator[](size_t i) const {
        return i < head.size() ? head[head.size() - i - 1] : tail[i - head.size()];
    }

    T &At(size_t i) {
        CheckIndex(i);
        return (*this)[i];
    }

    T &At(size_t i) const {
        CheckIndex(i);
        return (*this)[i];
    }

    T &Front() {
        return head.empty() ? tail.front() : head.back();
    }

    const T &Front() const {
        return head.empty() ? tail.front() : head.back();
    }

    T &Back() {
        return tail.empty() ? head.front() : tail.back();
    }

    T Back() const {
        return tail.empty() ? head.front() : tail.back();
    }

    void PushFront(const T &item) {
        head.push_back(item);
    }

    void PushBack(const T &item) {
        tail.push_back(item);
    }

    void Clear() {
        head.resize(0);
        tail.resize(0);
    }

    void CheckIndex(size_t i) const {
        if (i >= Size())
            throw out_of_range("Index is out of range");
    }

};