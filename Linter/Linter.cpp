#include "Linter.h"

Linter::Linter::Linter(const std::string& filename) {
    tokens = CodeParser::Token::tokenizeFile(filename, false,
                                             false, true,
                                             true, true,
                                             true, true);
    analyseTokens();
}

void Linter::Linter::printTokens() const {
    for (auto& t : tokens) {
        std::cout << CodeParser::type2string(t.getType()) << " " << t.getToken() << std::endl;
    }
}

bool Linter::Linter::areVariablesDefined() const {
    bool result = true;
    for (auto& p : variables) {
        result &= p.second;
    }
    return result;
}

void Linter::Linter::analyseTokens() {
    for (int i = 0; i < tokens.size() - 1; i++) {
        if (tokens[i].getType() == CodeParser::TokenType::VARIABLE_NAME) {
            int r = findVariable(tokens[i].getToken());
            if (r == -1)
                variables.emplace_back(std::make_pair(tokens[i].getToken(), tokens[i+1].getType() == CodeParser::TokenType::OPERATOR));
            else {
                variables[r].second = tokens[i+1].getType() == CodeParser::TokenType::OPERATOR;
            }
        }
    }
}

void Linter::Linter::detailedReport(std::ostream &out) {
    int c = 1;
    for(auto& p : variables) {
        out << c++ << ") " << p.first << " is " << (p.second ? "defined" : "undefined") << std::endl;
    }
}
