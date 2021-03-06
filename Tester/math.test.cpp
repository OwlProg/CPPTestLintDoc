#include "Tester.h"
#include "math.cpp"

Tester TEST_SYSTEM;

void checkFunction1() {
    TEST_SYSTEM.ToEqual(increment(4), 5);
}

void checkFunction2() {
    TEST_SYSTEM.ToEqual(increment(4), 6);
}

void checkFunction3() {
    TEST_SYSTEM.ToEqual(multiply2(4.0), 8.0001);
}

int main() {
    describe("!!!!!___MAIN___!!!!!");
    describe("Check inc()", checkFunction1);
    describe("Check int increment()", checkFunction2);
    describe("Check float multiply2()", checkFunction3);

    TEST_SYSTEM.printStat();
    return 0;
}