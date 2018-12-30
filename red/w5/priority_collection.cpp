#include "test_runner.h"
#include <memory>
#include <list>

using namespace std;

struct ItemPriority {
    int id;
    int priority;
};

bool operator<(const ItemPriority &lhs, const ItemPriority &rhs) {
    return lhs.priority < rhs.priority || (lhs.priority == rhs.priority && lhs.id < rhs.id);
}

template<typename T>
class PriorityCollection {

public:

    using Id = int;

    PriorityCollection() {
        items.reserve(1'000'000);
        items_priority.reserve(1'000'000);
        items_valid.reserve(1'000'000);
    }

    Id Add(T object) {
        items_priority[++sequence] = 0;
        items[sequence] = move(object);
        items_valid[sequence] = true;
        priority_queue.insert({sequence, 0});
        return sequence;
    }

    template<typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
        for (; range_begin != range_end; range_begin = next(range_begin), ++ids_begin)
            *ids_begin = Add(move(*range_begin));
    }

    bool IsValid(Id id) const {
        return items_valid[id];
    }

    const T &Get(Id id) const {
        return items[id];
    }

    void Promote(Id id) {
        priority_queue.erase({id, items_priority[id]});
        ++items_priority[id];
        priority_queue.insert({id, items_priority[id]});
    }

    pair<const T &, int> GetMax() const {
        const auto &max = *priority_queue.rbegin();
        const auto &item = items[max.id];
        return {item, max.priority};
    }

    pair<T, int> PopMax() {
        const auto max = *priority_queue.rbegin();
        items_valid[max.id] = false;
        priority_queue.erase(max);
        return {move(items[max.id]), max.priority};
    }

private:
    Id sequence = 0;

    set<ItemPriority> priority_queue;
    vector<T> items;
    vector<int> items_priority;
    vector<bool> items_valid;
};

class StringNonCopyable : public string {
public:
    using string::string;

    StringNonCopyable(const StringNonCopyable &) = delete;

    StringNonCopyable(StringNonCopyable &&) = default;

    StringNonCopyable &operator=(const StringNonCopyable &) = delete;

    StringNonCopyable &operator=(StringNonCopyable &&) = default;
};

void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto max = strings.GetMax();
        ASSERT_EQUAL(max.first, "red");
        ASSERT_EQUAL(max.second, 2);
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto max = strings.GetMax();
        ASSERT_EQUAL(max.first, "yellow");
        ASSERT_EQUAL(max.second, 2);

        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto max = strings.GetMax();
        ASSERT_EQUAL(max.first, "white");
        ASSERT_EQUAL(max.second, 0);
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }

}

void TestContainer() {
    PriorityCollection<StringNonCopyable> container;

    int id_1 = container.Add(StringNonCopyable("1"));
    int id_2 = container.Add(StringNonCopyable("2"));
    int id_3 = container.Add(StringNonCopyable("3"));

    {
        const auto max = container.GetMax();
        ASSERT_EQUAL(max.first, "3");
        ASSERT_EQUAL(max.second, 0);
    }

    container.Promote(id_1);

    {
        const auto max = container.GetMax();
        ASSERT_EQUAL(max.first, "1");
        ASSERT_EQUAL(max.second, 1);
    }

    container.Promote(id_2);
    container.Promote(id_3);
    container.Promote(id_3);
    container.Promote(id_3);
    container.Promote(id_3);
    container.Promote(id_1);

    {
        ASSERT_EQUAL(container.Get(id_3), "3");
    }

    ASSERT_EQUAL(container.IsValid(id_1), true);

    {
        const auto max = container.PopMax();
        ASSERT_EQUAL(max.first, "3");
        ASSERT_EQUAL(max.second, 4);
    }

    container.Promote(id_1);
    int id_4 = container.Add(StringNonCopyable("4"));
    id_3 = container.Add(StringNonCopyable("3"));

    {
        const auto max = container.PopMax();
        ASSERT_EQUAL(max.first, "1");
        ASSERT_EQUAL(max.second, 3);
    }

    {
        const auto max = container.PopMax();
        ASSERT_EQUAL(max.first, "2");
        ASSERT_EQUAL(max.second, 1);
    }

    {
        const auto max = container.PopMax();
        ASSERT_EQUAL(max.first, "3");
        ASSERT_EQUAL(max.second, 0);
    }

    {
        const auto max = container.PopMax();
        ASSERT_EQUAL(max.first, "4");
        ASSERT_EQUAL(max.second, 0);
    }
    ASSERT_EQUAL(container.IsValid(id_2), false);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    RUN_TEST(tr, TestContainer);
    return 0;
}