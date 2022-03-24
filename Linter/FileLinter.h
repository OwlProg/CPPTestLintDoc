#ifndef CPPTESTLINTDOC_FILELINTER_H
#define CPPTESTLINTDOC_FILELINTER_H

#include "../Utils/Tokenizer.h"
#include <ostream>

namespace Linter {

    class FileLinter {
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
        explicit FileLinter(const std::string& filename);

        bool areVariablesDefined() const;
        bool areFunctionsDefined() const;
        void detailedReport(std::ostream& out);
    };

}

#endif //CPPTESTLINTDOC_FILELINTER_H
