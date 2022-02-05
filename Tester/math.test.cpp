#include "Tester.h"
#include "math.cpp"

UnitResult checkFunction1() {
    return Tester::ToEqual(increment(4), 5);
}

UnitResult checkFunction2() {
    return Tester::T_ToEqual(increment(4), 5);
}

UnitResult checkFunction3() {
    return Tester::T_ToEqual(multiply2(4.0), 8.0);
}

int main() {
    describe("Check inc()", checkFunction1);
    describe("Check int increment() with ToEqual", checkFunction2);
    describe("Check float multiply() with T_ToEqual", checkFunction3);
    return 0;
}