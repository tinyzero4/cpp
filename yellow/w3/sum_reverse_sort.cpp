#include "sum_reverse_sort.h"
#include <algorithm>

int Sum(int x, int y) {
    return x + y;
}
string Reverse(string s) {
    int n = s.length();
    for (int i = 0; i < n / 2; i++)
        swap(s[i], s[n - i - 1]);
    return s;
}
void Sort(vector<int>& nums) {
    sort(nums.begin(), nums.end());
}