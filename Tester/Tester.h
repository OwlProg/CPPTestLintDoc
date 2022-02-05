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

    template <typename T> UnitResult T_ToEqual(T result, T expectedResult, double EPS = 1e4) {
        if (std::is_same<T, int>::value) {
            if (result == expectedResult) {
                return UnitResult::PASS;
            }
            else {
                return UnitResult::ERROR;
            }
        }

        if (std::is_same<T, double>::value) {
            if (abs(result - expectedResult) < EPS) {
                return UnitResult::PASS;
            }
            else {
                return UnitResult::ERROR;
            }
        }
    }
}



#endif //CPPTESTLINTDOC_TESTER_H
