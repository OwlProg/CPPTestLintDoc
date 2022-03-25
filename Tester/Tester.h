//
// Created by iiana on 20.01.2022.
//

#ifndef CPPTESTLINTDOC_TESTER_H
#define CPPTESTLINTDOC_TESTER_H

#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <ctime>

class Tester {
    private:
        int countPasses = 0;
        int countWarnings = 0;
        int countErrors = 0;
        unsigned int timeRun = 0;


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

        template <class T> bool isContainerClass() {
            return (std::is_same<T, std::vector<typename T::value_type>>::value);
        }

        template <typename T> void printUnitResult(UnitResult status, T result = T(), T expectedResult = T()) {
            switch(status) {
                case UnitResult::PASS:
                    std::cout << "PASS";
                    countPasses += 1;
                    break;
                case UnitResult::WARNING:
                    std::cout << "WARNING ";
                    countWarnings += 1;
                    if (isRealType<T>()) {
                        std::cout << "Expected: " << expectedResult << "\t" << "Received: " << result << "\n";
                    } else if (isContainerClass<T>()) {
                        std::cout << "Container is empty\n";
                    }
                    break;
                case UnitResult::ERROR:
                    std::cout << "ERROR ";
                    countErrors += 1;
                    if (isContainerClass<T>()) {
                        std::cout << "Value " << result << "is not in container\n";
                    } else {
                        std::cout << "Expected: " << expectedResult << "\t" << "Received: " << result << "\n";
                    }

                    break;
            }
        }

        template <typename T> void testEqualWithoutWarning(T result, T expectedResult) {
            if (result == expectedResult) {
                printUnitResult<T>(UnitResult::PASS);
            }
            else {
                printUnitResult(UnitResult::ERROR, result, expectedResult);
            }
        }

    public:
        /*
         * General
         */
        Tester() {
            srand(time(0));
            timeRun = clock();
        }

        void printStat() {
            std::cout << "The testing system was completed in " << timeRun / 1000.0 <<  " seconds\n";
            std::cout << "Count of PASS: " << countPasses <<  "\n";
            std::cout << "Count of WARN: " << countWarnings <<  "\n";
            std::cout << "Count of ERROR: " << countErrors <<  "\n";
        }



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
                testEqualWithoutWarning(result, expectedResult);
            } else if (isStringType<T>()) {
                testEqualWithoutWarning(result, expectedResult);
            } else if (isBoolType<T>()) {
                testEqualWithoutWarning(result, expectedResult);
            } else if (isNullType<T>()) {
                testEqualWithoutWarning(result, expectedResult);
            }
        }

        template <typename T> void ToCompare(std::string sign, T result, T expectedResult, double EPS = 1e-4) {
            if (sign == "==" || sign == "=") {
                ToEqual(result, expectedResult, EPS);
            } else if (sign == ">=") {
                if (result >= expectedResult) {
                    printUnitResult<T>(UnitResult::PASS);
                } else {
                    printUnitResult(UnitResult::ERROR, result, expectedResult);
                }
            } else if (sign == ">") {
                if (result > expectedResult) {
                    printUnitResult<T>(UnitResult::PASS);
                } else {
                    printUnitResult(UnitResult::ERROR, result, expectedResult);
                }
            } else if (sign == "<=") {
                if (result <= expectedResult) {
                    printUnitResult<T>(UnitResult::PASS);
                } else {
                    printUnitResult(UnitResult::ERROR, result, expectedResult);
                }
            } else if (sign == "<") {
                if (result < expectedResult) {
                    printUnitResult<T>(UnitResult::PASS);
                } else {
                    printUnitResult(UnitResult::ERROR, result, expectedResult);
                }
            }
        }

        void ToMatch(std::string str, std::string substr) {
            size_t pos = str.find(substr);
            if (pos != std::string::npos)
                printUnitResult<std::string>(UnitResult::PASS);
            else
                printUnitResult<std::string>(UnitResult::ERROR);
        }

};

void describe(const std::string& description, void (*function)() = nullptr);


#endif //CPPTESTLINTDOC_TESTER_H
