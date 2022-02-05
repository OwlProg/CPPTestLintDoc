#include "Tester.h"
#include "math.cpp"

UnitResult checkFunction1() {
    return Tester::ToEqual(increment(4), 5);
}

int main() {
    describe("Check inc()", checkFunction1);
    return 0;
}