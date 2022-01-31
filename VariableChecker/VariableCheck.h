#ifndef CPPTESTLINTDOC_VARIABLECHECK_H
#define CPPTESTLINTDOC_VARIABLECHECK_H

#include <vector>
#include <string>
#include "../Utils/Tokenizer.h"

class VariableCheck {
    std::vector<std::pair<std::string, bool>> varTable;
    std::string fname;
    int findVariable(const std::string& needle);
public:
    explicit VariableCheck(const std::string& filename);
    bool areVariablesInitialized();
};


#endif //CPPTESTLINTDOC_VARIABLECHECK_H
