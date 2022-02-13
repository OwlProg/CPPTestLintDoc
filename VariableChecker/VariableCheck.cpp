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
    bool funcArgs = false;
    bool maybeVar = false;
    auto v = CodeParser::Token::TokenizeFile(fname);
    for (int i = 0; i < v.size() - 1; i++) {
        if (v[i].getType() == CodeParser::TokenType::FUNCTION) {
            funcArgs = true;
        }
        if(v[i].getType() == CodeParser::TokenType::CLOSE_BRACKET && funcArgs) funcArgs = false;
        if (v[i].getType() == CodeParser::TokenType::VARIABLE && !funcArgs) {
            varTable.emplace_back(
                    std::make_pair(v[i].getToken(), v[i + 1].getType() == CodeParser::TokenType::OPERATOR));
        }
        if (v[i].getType() == CodeParser::TokenType::UNKNOWN && v[i + 1].getType() == CodeParser::TokenType::UNKNOWN) {
            maybeVar = true;
            continue;
        }
        if ((v[i + 1].getType() == CodeParser::TokenType::OPERATOR || v[i + 1].getType() == CodeParser::TokenType::SEMICOLON) && maybeVar) {
            varTable.emplace_back(
                    std::make_pair(v[i].getToken(), v[i + 1].getType() == CodeParser::TokenType::OPERATOR));
            maybeVar = false;
        }
        else if ((v[i + 1].getType() != CodeParser::TokenType::OPERATOR || v[i + 1].getType() != CodeParser::TokenType::SEMICOLON) && maybeVar){
            maybeVar = false;
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
