#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

using namespace std;

template<class T>
class ObjectPool {
public:
    T *Allocate() {
        if (free.empty()) {
            free.push(new T);
        }
        T *obj = free.front();
        free.pop();
        managed.insert(obj);
        return obj;
    }

    T *TryAllocate() {
        if (free.empty()) return nullptr;
        return this->Allocate();
    }

    void Deallocate(T *object) {
        if (managed.count(object) == 0) throw invalid_argument("not a managed object");
        managed.erase(object);
        free.push(object);
    }

    ~ObjectPool() {
        for (auto it = managed.begin(); it != managed.end(); it++) {
            delete *it;
        }
        while (!free.empty()) {
            delete free.front();
            free.pop();
        }
    }

private:
    queue<T *> free;
    set<T *> managed;
};


void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}