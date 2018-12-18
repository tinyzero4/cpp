#include "test_runner.h"

#include <vector>

using namespace std;

/**
Метод GetHead возвращает указатель на голову списка, он используется для перебора элементов списка (см. шаблон ToVector в заготовке решения)
Метод PushFront добавляет новый элемент в голову списка.
Метод InsertAfter вставляет новый элемент в список так, чтобы он шёл после узла node. Если node == nullptr, метод эквивалентен PushFront
Метод PopFront удаляет элемент из головы списка и освобождает выделенную под него память. Если список пуст, метод завершается корректно. Если после выполнения метода список стал пустым, метод GetHead должен возвращать nullptr
Метод RemoveAfter должен удалять из списка элемент, который следует за узлом node, и освобождать выделенную под него память. Если node == nullptr, метод эквивалентен PopFront. Если node->next == nullptr, то метод должен корректно завершаться.
Все методы, перечисленные выше, должны работать за O(1)
Деструктор класса LinkedList освобождает всю память, выделенную для хранения элементов списка.
 */

template<typename T>
class LinkedList {
public:
    struct Node {
        T value;
        Node *next = nullptr;
    };

    ~LinkedList() {
        while (head) {
            Node *node = head;
            head = head->next;
            delete node;
        }
    }

    void PushFront(const T &value) {
        Node *new_head = new Node;
        new_head->value = value;
        new_head->next = head;
        head = new_head;
    }

    void InsertAfter(Node *node, const T &value) {
        if (!node) {
            PushFront(value);
            return;
        }

        Node *new_node = new Node;
        new_node->value = value;
        new_node->next = node->next;
        node->next = new_node;
    }

    void RemoveAfter(Node *node) {
        if (!node) {
            PopFront();
            return;
        }

        if (!node->next) return;
        Node *next_node = node->next;
        node->next = next_node->next;
        delete next_node;
    }

    void PopFront() {
        if (!head) return;
        Node *old_head = head;
        head = head->next;
        delete old_head;
    }

    Node *GetHead() { return head; }

    const Node *GetHead() const { return head; }

private:
    Node *head = nullptr;
};

template<typename T>
vector<T> ToVector(const LinkedList<T> &list) {
    vector<T> result;
    for (auto node = list.GetHead(); node; node = node->next) {
        result.push_back(node->value);
    }
    return result;
}

void TestPushFront() {
    LinkedList<int> list;

    list.PushFront(1);
    ASSERT_EQUAL(list.GetHead()->value, 1);
    list.PushFront(2);
    ASSERT_EQUAL(list.GetHead()->value, 2);
    list.PushFront(3);
    ASSERT_EQUAL(list.GetHead()->value, 3);

    const vector<int> expected = {3, 2, 1};
    ASSERT_EQUAL(ToVector(list), expected);
}

void TestInsertAfter() {
    LinkedList<string> list;

    list.PushFront("a");
    auto head = list.GetHead();
    ASSERT(head);
    ASSERT_EQUAL(head->value, "a");

    list.InsertAfter(head, "b");
    const vector<string> expected1 = {"a", "b"};
    ASSERT_EQUAL(ToVector(list), expected1);

    list.InsertAfter(head, "c");
    const vector<string> expected2 = {"a", "c", "b"};
    ASSERT_EQUAL(ToVector(list), expected2);


}

void TestRemoveAfter() {
    LinkedList<int> list;
    for (int i = 1; i <= 5; ++i) {
        list.PushFront(i);
    }

    const vector<int> expected = {5, 4, 3, 2, 1};
    ASSERT_EQUAL(ToVector(list), expected);

    auto next_to_head = list.GetHead()->next;
    list.RemoveAfter(next_to_head);
    list.RemoveAfter(next_to_head);

    const vector<int> expected1 = {5, 4, 1};
    ASSERT_EQUAL(ToVector(list), expected1);

    while (list.GetHead()->next) {
        list.RemoveAfter(list.GetHead());
    }
    ASSERT_EQUAL(list.GetHead()->value, 5);

    LinkedList<int> l1;
    l1.PopFront();
    l1.PopFront();
    l1.PopFront();
    l1.PushFront(233);
    l1.PushFront(234);

    l1.GetHead()->next = nullptr;
    l1.InsertAfter(nullptr, 1233);
    l1.InsertAfter(nullptr, 1233);
    l1.InsertAfter(nullptr, 1233);
    l1.InsertAfter(nullptr, 1233);
    l1.InsertAfter(nullptr, 1233);

    l1.PopFront();
    l1.PopFront();
    l1.PopFront();
    l1.PopFront();

    l1.RemoveAfter(nullptr);
    l1.RemoveAfter(l1.GetHead());

    l1.InsertAfter(l1.GetHead(), 3);

    ASSERT_EQUAL(1, 1);


}

void TestPopFront() {
    LinkedList<int> list;

    for (int i = 1; i <= 5; ++i) {
        list.PushFront(i);
    }
    for (int i = 1; i <= 5; ++i) {
        list.PopFront();
    }
    ASSERT(list.GetHead() == nullptr);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestPushFront);
    RUN_TEST(tr, TestInsertAfter);
    RUN_TEST(tr, TestRemoveAfter);
    RUN_TEST(tr, TestPopFront);
    return 0;
}