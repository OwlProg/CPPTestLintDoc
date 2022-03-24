#include "Linter.h"

Linter::Linter::Linter(const std::string& filename) {
    tokens = CodeParser::Token::tokenizeFile(filename, false,
                                             false, true,
                                             true, true,
                                             true, true);
    analyseTokens();
}

bool Linter::Linter::areVariablesDefined() const {
    bool result = true;
    for (auto& p : variables) {
        result &= p.second;
    }
    return result;
}

void Linter::Linter::analyseTokens() {
    bool FuncOpen = false;
    std::string currFuncName;
    bool InsideBrackets = false;
    for (int i = 0; i < tokens.size() - 1; i++) {
        if (tokens[i].getType() == CodeParser::TokenType::VARIABLE_NAME && !InsideBrackets) {
            int r = find(tokens[i].getToken(), variables);
            if (r == -1)
                variables.emplace_back(std::make_pair(tokens[i].getToken(), tokens[i+1].getToken() == "="));
            else {
                variables[r].second = tokens[i+1].getToken() == "=";
            }
        }
        else if(tokens[i].getType() == CodeParser::TokenType::FUNCTION_NAME ||
                tokens[i].getType() == CodeParser::TokenType::UNKNOWN &&
                tokens[i + 1].getToken() == "(") {
            currFuncName = tokens[i].getToken();
            FuncOpen = true;
            InsideBrackets = true;
        }
        else if(tokens[i].getToken() == ")" && FuncOpen) {
            FuncOpen = false;
            InsideBrackets = false;
            int r = find(tokens[i].getToken(), functions);
            if (r == -1)
                functions.emplace_back(std::make_pair(currFuncName, tokens[i+1].getToken() == "{"));
            else {
                functions[r].second = tokens[i+1].getToken() == "{";
            }
        }
    }
}

void Linter::Linter::detailedReport(std::ostream &out) {
    int c = 1;
    out << "--- Variables ---" << std::endl;
    for(auto& p : variables) {
        out << c++ << ") " << p.first << " is " << (p.second ? "defined" : "undefined") << std::endl;
    }
    c = 1;
    out << "--- Functions ---" << std::endl;
    for(auto& p : functions) {
        out << c++ << ") " << p.first << " is " << (p.second ? "defined" : "undefined") << std::endl;
    }
}

bool Linter::Linter::areFunctionsDefined() const {
    bool result = true;
    for (auto& p : functions) {
        result &= p.second;
    }
    return result;
}
