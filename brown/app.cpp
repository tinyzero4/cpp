#include <string>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <cstring>
#include <utility>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

int main() {
    using namespace std;
    string sentence = "And I feel fine...";
    istringstream iss(sentence);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         ostream_iterator<string>(cout, "\n"));
}