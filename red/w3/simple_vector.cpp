#include "simple_vector.h"
#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using namespace std;


void TestConstruction() {
    SimpleVector<int> empty;
    ASSERT_EQUAL(empty.Size(), 0u);
    ASSERT_EQUAL(empty.Capacity(), 0u);
    ASSERT(empty.begin() == empty.end());

    SimpleVector<string> five_strings(5);
    ASSERT_EQUAL(five_strings.Size(), 5u);
    ASSERT(five_strings.Size() <= five_strings.Capacity());

    for (auto &item : five_strings) {
        ASSERT(item.empty());
    }

    five_strings[2] = "Hello";
    ASSERT_EQUAL(five_strings[2], "Hello");
}

void TestPushBack() {
    SimpleVector<int> v;
    for (int i = 10; i >= 1; --i) {
        v.PushBack(i);
        ASSERT(v.Size() <= v.Capacity());
    }
    sort(begin(v), end(v));

    const vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT_EQUAL(v.Size(), expected.size());
    ASSERT(equal(begin(v), end(v), begin(expected)));
}

void TestIteration() {
    SimpleVector<int> v(4);
    ASSERT_EQUAL(v.Size(), 4);
    ASSERT_EQUAL(v.Capacity(), 4);
    v[0] = 1;
    v[1] = 2;
    v[2] = 3;
    v[3] = 4;

    v.PushBack(123);
    ASSERT_EQUAL(v.Size(), 5);
    ASSERT_EQUAL(v.Capacity(), 8);


    cout << v[3] << endl;


//    for (int i = 0; i < 20; i++) {
//        v.PushBack(i);
//    }

//    ASSERT_EQUAL(15, v.Capacity());

    for (auto x: v) {
        cout << x << ", ";
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestConstruction);
    RUN_TEST(tr, TestPushBack);
//    RUN_TEST(tr, TestIteration);

    return 0;
}