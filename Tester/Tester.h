//
// Created by iiana on 20.01.2022.
//

#ifndef CPPTESTLINTDOC_TESTER_H
#define CPPTESTLINTDOC_TESTER_H

#include <string>

enum class UnitResult {
    PASS,
    WARNING,
    ERROR
};

void describe(const std::string& description, UnitResult (*function)());

namespace Tester {
    UnitResult ToEqual(int result, int expectedResult);
}



#endif //CPPTESTLINTDOC_TESTER_H
