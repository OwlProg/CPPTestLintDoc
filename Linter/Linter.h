#ifndef CPPTESTLINTDOC_LINTER_H
#define CPPTESTLINTDOC_LINTER_H

#include "../Utils/Tokenizer.h"
#include <ostream>

namespace Linter {

    class Linter {
        std::vector<CodeParser::Token> tokens;
        std::vector<std::pair<std::string, bool>> variables;
        std::vector<std::pair<std::string, bool>> functions;

        inline int find(const std::string& needle, std::vector<std::pair<std::string, bool>>& container) {
            for (int i = 0; i < container.size(); i++) {
                if (container[i].first == needle) return i;
            }
            return -1;
        };

        void analyseTokens();
    public:
        explicit Linter(const std::string& filename);

        void printTokens() const;

        bool areVariablesDefined() const;
        bool areFunctionsDefined() const;
        void detailedReport(std::ostream& out);
    };

}

#endif //CPPTESTLINTDOC_LINTER_H
