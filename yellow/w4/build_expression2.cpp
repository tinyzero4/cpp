#include <utility>
#include <vector>
#include <string>
#include <algorithm>
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
    string old_operation = "*";
    string new_operation;
    for (auto i = 0; i < n; ++i) {
        cin >> new_operation;
        if ((new_operation == "*" || new_operation == "/") &&
            (old_operation == "+" || old_operation == "-")) {
            result.push_front("(");
            result.push_back(")");
        }
        result.push_back(" ");
        result.push_back(new_operation);
        result.push_back(" ");
        int num;
        cin >> num;
        result.push_back(to_string(num));
        old_operation = new_operation;
    }
    for (auto i : result) {
        cout << i;
    }
    return 0;
}