#include <iostream>
#include "Utils/Tokenizer.h"
#include "VariableChecker/VariableCheck.h"

int main() {
    VariableCheck v("test.cpp");
    std::cout << v.areVariablesInitialized() << std::endl;
}