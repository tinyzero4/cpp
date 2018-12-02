#include "test_runner.h"

#include <ostream>

using namespace std;

#define PRINT_VALUES(out, x, y)  (out << (x) << endl << (y) << endl)

int main() {
    TestRunner tr;

    int x;
    cin >> x;

    if (x%2==0)
        PRINT_VALUES(cout, 1, 2);
    else
        PRINT_VALUES(cout, 3, 4);

    tr.RunTest([] {
        ostringstream output;
        PRINT_VALUES(output, 5, "red belt");
        ASSERT_EQUAL(output.str(), "5\nred belt\n");
    }, "PRINT_VALUES usage example");


}