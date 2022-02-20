//
// Created by iiana on 20.01.2022.
//

#ifndef CPPTESTLINTDOC_TESTER_H
#define CPPTESTLINTDOC_TESTER_H

#include <iostream>
#include <cmath>

class Tester {
    private:
        enum class UnitResult {
            PASS,
            WARNING,
            ERROR
        };

        template <typename T> bool isNullType() {
            return (std::is_same<T, std::nullptr_t>::value);
        }

        template <typename T> bool isIntegerType() {
            return (std::is_same<T, short int>::value ||
                    std::is_same<T, unsigned short int>::value ||
                    std::is_same<T, int>::value ||
                    std::is_same<T, unsigned int>::value ||
                    std::is_same<T, long long int>::value ||
                    std::is_same<T, unsigned long long int>::value ||
                    std::is_same<T, unsigned long int>::value);
        }

        template <typename T> bool isRealType() {
            return (std::is_same<T, float>::value ||
                    std::is_same<T, double>::value ||
                    std::is_same<T, long double>::value);
        }

        template <typename T> bool isBoolType() {
            return (std::is_same<T, bool>::value);
        }

        template <typename T> bool isCharType() {
            return (std::is_same<T, char>::value);
        }

        template <typename T> bool isStringType() {
            return (std::is_same<T, std::string>::value);
        }

        template <typename T> void printUnitResult(UnitResult status, T result = T(), T expectedResult = T()) {
            switch(status) {
                case UnitResult::PASS:
                    std::cout << "PASS";
                    break;
                case UnitResult::WARNING:
                    std::cout << "WARNING ";
                    if (isRealType<T>()) {
                        std::cout << "Expected: " << expectedResult << "\t" << "Received: " << result << "\n";
                    }
                    break;
                case UnitResult::ERROR:
                    std::cout << "ERROR ";
                    std::cout << "Expected: " << expectedResult << "\t" << "Received: " << result << "\n";
                    break;
            }
        }

        template <typename T> void checkWithoutWarning(T* result, T* expectedResult) {
            if (result == expectedResult) {
                printUnitResult<T>(UnitResult::PASS);
            }
            else {
                printUnitResult(UnitResult::ERROR, result, expectedResult);
            }
        }

    public:

        /*
         * Truthiness methods
         */
        template <typename T> void ToEqual(T result, T expectedResult, double EPS = 1e-4) {
            if (isRealType<T>()) {
                if (fabs(result - expectedResult) < EPS) {
                    double warningEPS = EPS * 1000;
                    if (fabs(result - expectedResult) < warningEPS) {
                        printUnitResult<T>(UnitResult::WARNING, result, expectedResult);
                    } else {
                        printUnitResult<T>(UnitResult::PASS);
                    }
                } else {
                    printUnitResult(UnitResult::ERROR, result, expectedResult);
                }
            } else if (isIntegerType<T>()) {
                checkWithoutWarning(result, expectedResult);
            } else if (isStringType<T>()) {
                checkWithoutWarning(result, expectedResult);
            } else if (isBoolType<T>()) {
                checkWithoutWarning(result, expectedResult);
            } else if (isNullType<T>()) {
                checkWithoutWarning(result, expectedResult);
            }
        }

//        template <typename T> void ToCompare(std::string sign, T result, T expectedResult, double EPS = 1e-4) {
//            switch (sign) {
//                case "==":
//
//                    break;
//                default:
//            }
//        }

};

void describe(const std::string& description, void (*function)() = nullptr);


#endif //CPPTESTLINTDOC_TESTER_H
