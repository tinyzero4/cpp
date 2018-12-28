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
        priority_queue[0].push_back(o_id);
        ids.insert(o_id);
        return o_id;
    }

    template<typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
        for (; range_begin != range_end; range_begin = next(range_begin), ++ids_begin) {
            Id o_id = sequence++;
            *ids_begin = o_id;
            items[o_id] = {move(*range_begin), 0};
            priority_queue[0].push_back(o_id);
            ids.insert(o_id);
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return ids.find(id) != ids.end();
    }

    // Получить объект по идентификатору
    const T &Get(Id id) const {
        return items.at(id).value;
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        PrioritizedValue value = move(items[id]);
        priority_queue[value.priority].remove(id);
        ++value.priority;
        priority_queue[value.priority].push_back(id);
        max_priority = max(max_priority, value.priority);
        items[id] = move(value);
    }


    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T &, int> GetMax() const {
        const Id &last = priority_queue.at(max_priority).back();
        const T &item = items.at(last).value;
        return {item, max_priority};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        list<Id> &priority_ids = priority_queue.at(max_priority);
        Id id = priority_ids.front();
        priority_ids.pop_front();
        ids.erase(id);

        while (priority_ids.empty() && max_priority > 0) {
            --max_priority;
            priority_ids = priority_queue.at(max_priority);
        }

        PrioritizedValue value = move(items[id]);
        return {move(value.value), value.priority};
    }


private:
    Id sequence = 0;
    int max_priority = 0;
    map<Id, PrioritizedValue> items;
    set<Id> ids;
    map<int, list<Id>> priority_queue;
};


class StringNonCopyable : public string {
public:
    using string::string;  // Позволяет использовать конструкторы строки
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
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }

}

void printMax(const PriorityCollection<StringNonCopyable> &container) {
    const pair<const StringNonCopyable &, int> &pair = container.GetMax();
    cout << pair.first << " - " << pair.second << endl;
}

void TestContainer() {
    PriorityCollection<StringNonCopyable> container;

    size_t id_1 = container.Add(StringNonCopyable("1"));
    size_t id_2 = container.Add(StringNonCopyable("2"));

    printMax(container);
//    auto max1 = container.PopMax();
//    cout << max1.first << " - " << max1.second << endl;

    container.Promote(0);

    cout << container.GetMax().first << " - " << container.GetMax().second << endl;
    auto max1 = container.PopMax();
    cout << max1.first << " - " << max1.second << " ";
    cout << container.GetMax().first << " - " << container.GetMax().second << endl;



    /**
     * 1. {"1", 0}, {"2", 0}

Promote("1"): {"2", 0}, {"1", 1}

Promote("2"): {"1", 1}, {"2", 1}

GetMax(): {"2", 1}


     */







}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    RUN_TEST(tr, TestContainer);
    return 0;
}