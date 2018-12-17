#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

template<typename T>
class SimpleVector {
public:
    SimpleVector() {
        data = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    explicit SimpleVector(size_t size) {
        data = new T[size];
        size_ = 0;
        capacity_ = size;
    }

    ~SimpleVector();

    T &operator[](size_t index) {
        if (data == nullptr || index > Size()) throw invalid_argument("invalid index");
        return *(data + index);
    }

    T *begin();

    T *end();

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    void PushBack(const T &value) {
        if (Size() == Capacity()) {
            T* old = data;
            size_t capacity = (data == nullptr) ? 1 : Capacity() * 2;
            data = new T[capacity];

        }
    }

private:
    T *data;
    size_t size_;
    size_t capacity_;
};