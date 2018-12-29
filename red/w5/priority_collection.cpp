#include "test_runner.h"
#include <memory>
#include <list>

using namespace std;

template<typename T>
class PriorityCollection {

public:

    using Id = size_t;

    struct PrioritizedValue {
        T value;
        int priority;
    };

    Id Add(T object) {
        const Id o_id = sequence++;
        items[o_id] = {move(object), 0};
        priority_queue[0].insert(o_id);
        return o_id;
    }

    template<typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
        for (; range_begin != range_end; range_begin = next(range_begin), ++ids_begin) {
            Id o_id = sequence++;
            *ids_begin = o_id;
            items[o_id] = {move(*range_begin), 0};
            priority_queue[0].insert(o_id);
        }
    }

    bool IsValid(Id id) const {
        return items.count(id) > 0;
    }

    const T &Get(Id id) const {
        return items.at(id).value;
    }

    void Promote(Id id) {
        PrioritizedValue value = move(items[id]);
        priority_queue[value.priority].erase(id);

        ++value.priority;

        priority_queue[value.priority].insert(id);

        max_priority = max(max_priority, value.priority);

        items[id] = move(value);
    }


    pair<const T &, int> GetMax() const {
        const Id &last = *(prev(priority_queue.at(max_priority).end()));
        const T &item = items.at(last).value;
        return {item, max_priority};
    }

    pair<T, int> PopMax() {
        set<Id> &priority_ids = priority_queue.at(max_priority);

        const auto &max = prev(priority_ids.end());

        Id id = *max;
        priority_ids.erase(max);

        while (priority_ids.empty() && max_priority > 0) {
            --max_priority;
            priority_ids = priority_queue.at(max_priority);
        }

        PrioritizedValue value = move(items[id]);
        pair<T, int> res = {move(value.value), value.priority};

        items.erase(id);

        return res;
    }

private:
    Id sequence = 0;
    int max_priority = 0;
    map<Id, PrioritizedValue> items;
    map<int, set<Id>> priority_queue;
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

    size_t id_1 = container.Add(StringNonCopyable("1"));
    size_t id_2 = container.Add(StringNonCopyable("2"));
    size_t id_3 = container.Add(StringNonCopyable("3"));

    container.Promote(id_1);
    container.Promote(id_2);
    container.Promote(id_3);
    container.Promote(id_3);
    container.Promote(id_3);
    container.Promote(id_3);
    container.Promote(id_1);

    {
        const auto max = container.PopMax();
        ASSERT_EQUAL(max.first, "3");
        ASSERT_EQUAL(max.second, 4);
    }#include "test_runner.h"
#include <memory>
#include <list>

    using namespace std;

    template<typename T>
    class PriorityCollection {

    public:

        using Id = size_t;

        struct PrioritizedValue {
            T value;
            int priority;
        };

        Id Add(T object) {
            const Id o_id = sequence++;
            items[o_id] = {move(object), 0};
            priority_queue[0].insert(o_id);
            return o_id;
        }

        template<typename ObjInputIt, typename IdOutputIt>
        void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
            for (; range_begin != range_end; range_begin = next(range_begin), ++ids_begin) {
                Id o_id = sequence++;
                *ids_begin = o_id;
                items[o_id] = {move(*range_begin), 0};
                priority_queue[0].insert(o_id);
            }
        }

        bool IsValid(Id id) const {
            return items.count(id) > 0;
        }

        const T &Get(Id id) const {
            return items.at(id).value;
        }

        void Promote(Id id) {
            PrioritizedValue value = move(items[id]);
            priority_queue[value.priority].erase(id);

            ++value.priority;

            priority_queue[value.priority].insert(id);

            max_priority = max(max_priority, value.priority);

            items[id] = move(value);
        }


        pair<const T &, int> GetMax() const {
            const Id &last = *(prev(priority_queue.at(max_priority).end()));
            const T &item = items.at(last).value;
            return {item, max_priority};
        }

        pair<T, int> PopMax() {
            set<Id> &priority_ids = priority_queue.at(max_priority);

            const auto &max = prev(priority_ids.end());

            Id id = *max;
            priority_ids.erase(max);

            while (priority_ids.empty() && max_priority > 0) {
                --max_priority;
                priority_ids = priority_queue.at(max_priority);
            }

            PrioritizedValue value = move(items[id]);
            pair<T, int> res = {move(value.value), value.priority};

            items.erase(id);

            return res;
        }

    private:
        Id sequence = 0;
        int max_priority = 0;
        map<int, PrioritizedValue> items;
        map<int, set<Id>> priority_queue;
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

        size_t id_1 = container.Add(StringNonCopyable("1"));
        size_t id_2 = container.Add(StringNonCopyable("2"));
        size_t id_3 = container.Add(StringNonCopyable("3"));

        container.Promote(id_1);
        container.Promote(id_2);
        container.Promote(id_3);
        container.Promote(id_3);
        container.Promote(id_3);
        container.Promote(id_3);
        container.Promote(id_1);

        {
            const auto max = container.PopMax();
            ASSERT_EQUAL(max.first, "3");
            ASSERT_EQUAL(max.second, 4);
        }

        container.Promote(id_1);
        size_t id_4 = container.Add(StringNonCopyable("4"));
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
    }

    int main() {
        TestRunner tr;
        RUN_TEST(tr, TestNoCopy);
        RUN_TEST(tr, TestContainer);
        return 0;
    }

    container.Promote(id_1);
    size_t id_4 = container.Add(StringNonCopyable("4"));
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
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    RUN_TEST(tr, TestContainer);
    return 0;
}