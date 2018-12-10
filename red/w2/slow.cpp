#include <iomanip>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include "profile.h"


using namespace std;

class ReadingManager {
public:
    ReadingManager() : page_rating(PAGES_COUNT) {}

    void Read(int user_id, int page_count) {
        if (readers.count(user_id) == 0) {
            readers[user_id] = page_count;
            if (page_count < PAGES_COUNT)
                for (int i = page_count; i < PAGES_COUNT; i++) page_rating[i]++;
        } else {
            int prev_page = readers[user_id];
            if (prev_page < PAGES_COUNT)
                for (int i = prev_page; i < page_count; i++) page_rating[i]--;
            readers[user_id] = page_count;
        }
    }

    double Cheer(int user_id) const {
        if (readers.count(user_id) == 0) return 0;

        const unsigned long total_readers = readers.size();
        if (total_readers == 1) {
            return 1;
        }

        int rating = page_rating[readers.at(user_id) - 1];

        if (rating == total_readers) {
            return 0;
        }

        return (rating) * 1.0 / (total_readers - 1);
    }

private:
    static const int PAGES_COUNT = 1000;

    vector<int> page_rating;
    map<int, int> readers;

};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}