/*!
    \author Kozulin Ilya
    \warning tests needed; generating .html, .md and .pdf still not implemented
*/

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
        COMMENT,
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

        std::string type() const;

        std::string getToken() const;

        TokenType getType() const;

        void setToken(const std::string &str);

        void setType(const TokenType &_type);

        static Token joinTokens(const Token &token1, const Token &token2, bool space);

        static void CombineKeywords(std::vector<Token> &tokens);

        static void CombineComments(std::vector<Token> &tokens);

        static void CombineTypes(std::vector<Token> &tokens);

        static void CombineOperators(std::vector<Token> &tokens);

        static std::vector<Token> TokenizeText(const std::string &code, bool spaces = false, bool tabs = false, bool new_lines = false);

        static std::vector<Token> TokenizeFile(const std::string &FileName, bool spaces = false, bool tabs = false, bool new_lines = false);

    };
}
#endif //CPPTESTLINTDOC_TOKENIZER_H
