//
// Created by iiana on 20.01.2022.
//

#include "Tester.h"
#include <iostream>

void describe(const std::string& description, UnitResult (*function)()) {
    std::cout << description << '\n';
    UnitResult result = function();
    switch(result) {
        case UnitResult::PASS:
            std::cout << "PASS\n";
            break;
        case UnitResult::WARNING:
            std::cout << "WARNING\n";
            break;
        case UnitResult::ERROR:
            std::cout << "ERROR\n";
            break;
    }
}

UnitResult Tester::ToEqual(int result, int expectedResult) {
    if (result == expectedResult) return UnitResult::PASS;
    else return UnitResult::ERROR;
}