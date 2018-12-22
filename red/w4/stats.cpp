#include "stats.h"
#include <vector>

using namespace std;

Stats::Stats() {
    this->methodStats = {
            {"GET",     0},
            {"PUT",     0},
            {"POST",    0},
            {"DELETE",  0},
            {"UNKNOWN", 0},
    };
    this->uriStats = {
            {"/",        0},
            {"/order",   0},
            {"/product", 0},
            {"/basket",  0},
            {"/help",    0},
            {"unknown",  0},
    };
}

void Stats::AddMethod(string_view method) {
    string method_str = string(method);
    string method_type = (methodStats.count(method_str) != 0) ? method_str : "UNKNOWN";
    ++methodStats[method_type];
}

void Stats::AddUri(string_view uri) {
    string uri_str = string(uri);
    string uri_type = (uriStats.count(uri_str) != 0) ? uri_str : "unknown";
    ++uriStats[uri_type];
}

const map<string_view, int> &Stats::GetMethodStats() const {
    return this->methodStats;
}

const map<string_view, int> &Stats::GetUriStats() const {
    return this->uriStats;
}

HttpRequest ParseRequest(string_view line) {
    vector<string_view> data;
    data.reserve(3);

    size_t end = 0;
    while (!line.empty() && isspace(line[0])) line.remove_prefix(1);

    while ((end = line.find(' ')) != string_view::npos) {
        data.push_back(line.substr(0, end));
        line = line.substr(end + 1);
    }

    data.push_back(line);

    return {data[0], data[1], data[2]};
}