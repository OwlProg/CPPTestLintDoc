//
// Created by iiana on 20.01.2022.
//

#include "Tester.h"

void describe(const std::string &description, void (*function)()) {
    if (function == nullptr) {
        std::cout << description << '\n';
    } else {
        function();
        std::cout << ": " << description << "\n\n";
    }
}