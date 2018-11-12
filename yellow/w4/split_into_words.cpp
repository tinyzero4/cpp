#include <string>
#include <vector>
#include "iostream"
#include "algorithm"

using namespace std;

vector<string> SplitIntoWords(const string &s) {
    vector<string> res;
    auto str_end = find(s.begin(), s.end(), ' ');
    auto str_start = s.begin();
    while (str_end != s.end()) {
        res.push_back(string(str_start, str_end));
        str_start = str_end + 1;
        str_end = find(str_end + 1, s.end(), ' ');
    }

    res.push_back(string(str_start, str_end));

    return res;
}

int main() {
    string s = "C Cpp Java Python";

    vector<string> words = SplitIntoWords(s);
    cout << words.size() << " ";
    for (auto it = begin(words); it != end(words); ++it) {
        if (it != begin(words)) {
            cout << "/";
        }
        cout << *it;
    }
    cout << endl;

    return 0;
}