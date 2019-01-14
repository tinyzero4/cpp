#include "test_runner.h"
#include "profile.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <functional>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};

class Database {
public:
    bool Put(const Record& record) {
        auto [it, inserted] = storage.insert({record.id, RecordData{record, {}, {}, {}}});
        if (!inserted) return false;

        auto& data = it->second;
        const Record* r_ptr = &data.record;
        data.timestamp_iter = by_timestamp_idx.insert({record.timestamp, r_ptr});
        data.karma_iter = by_karma_idx.insert({record.karma, r_ptr});
        data.user_iter = by_user_idx.insert({record.user, r_ptr});
        return true;
    }

    const Record* GetById(const string& id) const {
        auto const it = storage.find(id);
        if (it == storage.end()) return nullptr;
        return &it->second.record;
    }

    bool Erase(const string& id) {
        auto const it = storage.find(id);
        if (it == storage.end()) return false;

        auto data = it->second;
        by_timestamp_idx.erase(data.timestamp_iter);
        by_karma_idx.erase(data.karma_iter);
        by_user_idx.erase(data.user_iter);

        storage.erase(it);
        return true;
    }

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
        auto end = by_timestamp_idx.end();
        for (auto it = by_timestamp_idx.lower_bound(low); it != end && it->first <= high; ++it) {
            if (!callback(*it->second)) return;
        }
    }

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
        auto end = by_karma_idx.end();
        for (auto it = by_karma_idx.lower_bound(low); it != end && it->first <= high; ++it) {
            if (!callback(*it->second)) return;
        }
    }

    template <typename Callback>
    void AllByUser(const string& user, Callback callback) const {
        auto end = by_user_idx.end();
        for (auto it = by_user_idx.lower_bound(user); it != end && it->first == user; ++it) {
            if (!callback(*it->second)) return;
        }
    }

private:
    template <typename Type>
    using Index = multimap<Type, const Record*>;

    struct RecordData {
        Record record;
        Index<int>::iterator timestamp_iter;
        Index<int>::iterator karma_iter;
        Index<string>::iterator user_iter;
    };

    unordered_map<string, RecordData> storage;
    Index<int> by_timestamp_idx;
    Index<int> by_karma_idx;
    Index<string> by_user_idx;
};

void TestRangeBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
    db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

    int count = 0;
    db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestSameUser() {
    Database db;
    db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
    db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

    int count = 0;
    db.AllByUser("master", [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestReplacement() {
    const string final_body = "Feeling sad";

    Database db;
    db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
    db.Erase("id");
    db.Put({"id", final_body, "not-master", 1536107260, -10});

    auto record = db.GetById("id");
    ASSERT(record != nullptr);
    ASSERT_EQUAL(final_body, record->title);
}

string GenerateRandomString(const size_t len) {
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string res;

    for (int i = 0; i < len; ++i) res.append(1, alphanum[rand() % (sizeof(alphanum) - 1)]);
    return res;
}

Record GenerateRecord() {
    return {
        "id_" + GenerateRandomString(10),
        "title" + GenerateRandomString(10),
        "user" + GenerateRandomString(10),
        rand(),
        rand()
    };
}

void GenerateRecords(Database &db) {
    for (int i = 0; i < 1'000'000; i++) {
        db.Put(GenerateRecord());
    }
}

void TestPerformance() {
    Database db;
    {
        LOG_DURATION("test insertion 1'000'000 records");
        GenerateRecords(db);
    }

    {
        LOG_DURATION("test range by karma");
        for (int i = 0; i < 100; i++) {
            LOG_DURATION("iteration_" + i);
            size_t count = 0;
            int low = rand();
            int high = rand();
            db.RangeByKarma(rand(), rand(), [&count] (const Record &record) {
                count++;
                return true;
            });
        }
    }

}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);
    return 0;
}