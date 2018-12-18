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
        size = 0;
        capacity = 0;
    }

    explicit SimpleVector(size_t size) {
        data = new T[size];
        size = size;
        capacity = size;
    }

    ~SimpleVector() {
        delete[] data;
    }

    T &operator[](size_t index) {
        if (Size() == 0 || index >= Size()) throw invalid_argument("invalid index");
        return *(data + index);
    }

    T *begin() {
        return data;
    }

    T *end() {
        return data + Size();
    }

    size_t Size() const {
        return size;
    }

    size_t Capacity() const {
        return capacity;
    }

    void PushBack(const T &value) {
        if (Size() == Capacity()) resize();
        data[size++] = value;
    }

private:
    T *data;
    size_t size;
    size_t capacity;

    void resize() {
        size_t new_capacity = Capacity() == 0 ? 1 : Capacity() * 2;

        T* new_data= new T[new_capacity];
        if (Capacity() != 0) {
            copy(begin(), end(), new_data);
            delete[] data;
        }

        data = new_data;
        capacity = new_capacity;
    }

};