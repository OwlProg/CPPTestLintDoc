#ifndef CPPTESTLINTDOC_TOKENIZER_H
#define CPPTESTLINTDOC_TOKENIZER_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

namespace CodeParser
{
    enum class TokenType
    {
        TYPENAME,
        LITERAL,
        KEYWORD,
        STRING,
        CLASS,
        STRUCT,
        FUNCTION,
        VARIABLE,
        OPERATOR,
        SEMICOLON,
        OPEN_BRACKET,
        CLOSE_BRACKET,
        PASS_SYMBOL,
        UNKNOWN
    };

    class Token
    {
    private:

        TokenType token_type;

        std::string token_value;

    public:

        Token();

        explicit Token(const std::string &str);

        std::string type();

        std::string getToken() const;

        TokenType getType() const;

        void setToken(const std::string &str);
    };

    void CombineTypes(std::vector<Token>& tokens);

    void CombineOperators(std::vector<Token>& tokens);

    std::vector<Token> TokenizeText(const std::string &code, bool spaces = false, bool tabs = false, bool new_lines = false);

    std::vector<Token> TokenizeFile(const std::string &FileName, bool spaces = false, bool tabs = false, bool new_lines = false);
}

#endif //CPPTESTLINTDOC_TOKENIZER_H
