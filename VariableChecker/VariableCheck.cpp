#include "VariableCheck.h"

VariableCheck::VariableCheck(const std::string &filename) : fname(filename) {

}

int VariableCheck::findVariable(const std::string& needle) {
    for (int i = 0; i < varTable.size(); i++) {
        if (varTable[i].first == needle) return i;
    }
    return -1;
}

bool VariableCheck::areVariablesInitialized() {
    auto v = CodeParser::Token::TokenizeFile(fname);
    for (int i = 0; i < v.size() - 1; i++) {
        if (v[i].getType() == CodeParser::TokenType::VARIABLE) {
            varTable.emplace_back(
                    std::make_pair(v[i].getToken(), v[i + 1].getType() == CodeParser::TokenType::OPERATOR));
        }
        if (v[i].getType() == CodeParser::TokenType::UNKNOWN
            && v[i + 1].getType() == CodeParser::TokenType::OPERATOR) {
            int idx = findVariable(v[i].getToken());
            if (idx != -1) {
                varTable[idx].second = true;
            }
        }
    }
    bool result = true;
    for (auto x : varTable) {
        result &= x.second;
    }
    return result;
}
