#include <string>
#include <iostream>
#include <deque>

using namespace std;

int main() {
    deque<string> result;
    int number;
    cin >> number;
    result.push_back(to_string(number));
    int n;
    cin >> n;
    for (auto i = 0; i < n; ++i) {
        result.push_front("(");
        result.push_back(") ");
        string operation;
        cin >> operation;
        result.push_back(operation);
        result.push_back(" ");
        int num;
        cin >> num;
        result.push_back(to_string(num));
    }
    for (auto i : result) {
        cout << i;
    }
    return 0;
}