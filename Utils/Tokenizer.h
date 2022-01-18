/*!
    @author Kozulin Ilya
    @brief utility for easy code parsing
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
#include <unordered_map>

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
        UNION,
        ENUM,
        ALIAS,
        FUNCTION,
        VARIABLE,
        OPERATOR,
        SEMICOLON,
        OPEN_BRACKET,
        CLOSE_BRACKET,
        PASS_SYMBOL,
        COMMENT,
        USER_TYPE,
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

        /*!
         * @brief finds vector of names of user types in the code
         *
         * @param tokens
         * @return vector of names of user types
         */
        static std::vector<std::string> findUserTypes(const std::vector<Token> &tokens);

        /*!
         * @brief sets token with any user type tokentype "USER_TYPE" instead of "UNKNOWN"
         *
         * @param tokens vector of tokens processed from code
         * @param user_types vector of names of user types
         */
        static void setUserTokenTypes(std::vector<Token> &tokens, const std::vector<std::string> &user_types);

        /*!
         * @brief makes from two tokens one combined if they have the same token type, else returns empty token
         *
         * @param token1 first token to combine
         * @param token2 second token to combine
         * @param space flag - true if space between token value needed
         * @return combined token
         */
        static Token joinTokens(const Token &token1, const Token &token2, bool space);

        /*!
         * @brief changes macros and alias types to the type of token they are switched with instead of "UNKNOWN"
         *
         * @param tokens vector of tokens processed from code
         */
        static void changeAliasAndMacroTokens(std::vector<Token> &tokens);

        /*!
         * @brief combines each pair of consecutive tokens with keywords ("enum", "class" -> "enum class")
         *
         * @param tokens std::vector of tokens made from text of code
         */
        static void CombineKeywords(std::vector<Token> &tokens);

        /*!
         * @brief combines several comment tokens into one
         *
         * @param tokens std::vector of tokens made from text of code
         */
        static void CombineComments(std::vector<Token> &tokens);

        /*!
         * @brief combines each pair of consecutive tokens with typenames ("unsigned", "long", "long" -> "unsigned long long")
         *
         * @param tokens std::vector of tokens made from text of code
         */
        static void CombineTypes(std::vector<Token> &tokens);

        /*!
         * @brief combines each pair of consecutive tokens with operators ("+", "=" -> "+=")
         *
         * @param tokens std::vector of tokens made from text of code
         */
        static void CombineOperators(std::vector<Token> &tokens);

        /*!
         * @brief emplaces currnet token taking into account oportunity of having an empty string or having a semicolon in string which actually makes
         *        it two different tokens
         *
         * @param tokens vector of tokens generated from the code
         * @param token a token to add
         */
        static void AddToken(std::vector<Token> &tokens, const std::string &token);

        /*!
         * @brief makes from a text of code vector of tokens
         *
         * @param code text of code
         * @param spaces flag if it is necessary to process spaces (" ") as separate tokens
         * @param tabs flag if it is necessary to process tabs ("\t") as separate tokens
         * @param new_lines flag if it is necessary to process line breaks ("\n") as separate tokens
         * @param combineKeywords flag if it is necessary to combine keywords ("static", "void" -> "static void")
         * @param combineComments flag if it is necessary to combine comments ("//", "comment1" -> "// comment1")
         * @param combineTypes flag if it is necessary to combine typenames ("unsigned", "long", "long" -> "unsigned long long")
         * @param combineOperators flag if it is necessary to combine operators ("+", "=" -> "+=")
         * @return std::vector of tokens
         */
        static std::vector<Token>
        TokenizeText(const std::string &code, bool spaces = false, bool tabs = false, bool new_lines = false, bool combineKeywords = false, bool combineComments = false,
                     bool combineTypes = false, bool combineOperators = false);

        /*!
         * @brief makes from a text of code vector of tokens
         *
         * @param FileName file with code to tokenize
         * @param spaces flag if it is necessary to process spaces (" ") as separate tokens
         * @param tabs flag if it is necessary to process tabs ("\t") as separate tokens
         * @param new_lines flag if it is necessary to process line breaks ("\n") as separate tokens
         * @param combineKeywords flag if it is necessary to combine keywords ("static", "void" -> "static void")
         * @param combineComments flag if it is necessary to combine comments ("//", "comment1" -> "// comment1")
         * @param combineTypes flag if it is necessary to combine typenames ("unsigned", "long", "long" -> "unsigned long long")
         * @param combineOperators flag if it is necessary to combine operators ("+", "=" -> "+=")
         * @return std::vector of tokens
         */
        static std::vector<Token> TokenizeFile(const std::string &FileName, bool spaces = false, bool tabs = false, bool new_lines = false, bool combineKeywords = false,
                                               bool combineComments = false, bool combineTypes = false, bool combineOperators = false);

    };
}
#endif //CPPTESTLINTDOC_TOKENIZER_H
