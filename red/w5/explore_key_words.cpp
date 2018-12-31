#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <future>

using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator+=(const Stats &other) {
        for (auto &p : other.word_frequences) {
            word_frequences[p.first] += p.second;
        }
    }
};

Stats ExploreLine(const set<string> &key_words, const string &line) {
    Stats stats;
    std::istringstream ss(line);
    std::string sentence;

    for (string word; getline(ss, word, ' ');) {
        if (key_words.find(word) != key_words.end()) {
            stats.word_frequences[word] += 1;
        }
    }
    return stats;
}

Stats ExploreKeyWordsSingleThread(
        const set<string> &key_words, istream &input
) {
    Stats result;
    for (string line; getline(input, line);) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

Stats ExploreKeyWords(const set<string> &key_words, istream &input) {
    Stats result;

    vector<future<Stats>> mappers;
    stringstream buffer;
    int buffer_size = 0;
    const int max_buffer_size = 1'000;
    for (string line; getline(input, line);) {
        if (buffer_size == max_buffer_size) {
            mappers.push_back(async(ExploreLine, ref(key_words), buffer.str()));
            buffer.str(string());
            buffer_size = 0;
        }
        ++buffer_size;
        buffer << line << " ";
    }

    if (!buffer.str().empty()) mappers.push_back(async(ExploreLine, ref(key_words), buffer.str()));

    for (auto &r : mappers) {
        result += r.get();
    }

    return result;
}

void TestBasic() {
    const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
            {"yangle", 6},
            {"rocks",  2},
            {"sucks",  1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
}