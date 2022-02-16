/*!
    @author Ailurus
*/

#ifndef CPPTESTLINTDOC_UTILS_H
#define CPPTESTLINTDOC_UTILS_H

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <unordered_set>

namespace CodeParser {

    enum class TokenType {
        TYPENAME,
        LITERAL,
        KEYWORD,
        STRING,
        CLASS_DEF,
        STRUCT_DEF,
        UNION_DEF,
        ENUM_DEF,
        ALIAS,
        FUNCTION_NAME,
        VARIABLE_NAME,
        OPERATOR,
        SEMICOLON,
        OPEN_BRACKET,
        CLOSE_BRACKET,
        PASS_SYMBOL,
        COMMENT,
        PREPROCESSOR_DIRECTIVE,
        UNKNOWN
    };

    std::string type2string(const TokenType &tokenType);

    class Token {

    private:

        TokenType token_type;

        std::string token_value;

    public:

        Token();

        explicit Token(const std::string &str);

        std::string getToken() const;

        TokenType getType() const;

        void setToken(const std::string &str);

        void setType(const TokenType &type);

        /*!
         * @brief finds vector of names of user types in the code
         *
         * @param tokens
         * @return vector of names of user types
         */
        static std::vector<std::string> findUserTypes(const std::vector<Token> &tokens);

        /*!
         * @brief sets token with any user type token type "USER_TYPE" instead of "UNKNOWN"
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
        static void combineKeywords(std::vector<Token> &tokens);

        /*!
         * @brief combines several comment tokens into one
         *
         * @param tokens std::vector of tokens made from text of code
         */
        static void combineComments(std::vector<Token> &tokens);

        /*!
         * @brief combines each pair of consecutive tokens with type names ("unsigned", "long", "long" -> "unsigned long long")
         *
         * @param tokens std::vector of tokens made from text of code
         */
        static void combineTypes(std::vector<Token> &tokens);

        /*!
         * @brief combines each pair of consecutive tokens with operators ("+", "=" -> "+=")
         *
         * @param tokens std::vector of tokens made from text of code
         */
        static void combineOperators(std::vector<Token> &tokens);

        /*!
         * @brief emplace current token taking into account opportunity of having an empty string or having a semicolon in string which actually makes
         *        it two different tokens
         *
         * @param tokens vector of tokens generated from the code
         * @param token a token to add
         */
        static void addToken(std::vector<Token> &tokens, const std::string &token);

        /*!
         * @brief finds all initialized variables and functions, and sets correct type of their tokens
         *
         * @param tokens std::vector of tokens made from text of code
         */
        static void setVariablesAndFunctionsTokenTypes(std::vector<Token> &tokens);

        /*!
         * @brief finds all class names
         *
         * @param tokens std::vector of tokens made from text of code
         */
        static void setClassNameTokenTypes(std::vector<Token> &tokens);

        /*!
         * @brief makes from a text of code vector of tokens
         *
         * @param code text of code
         * @param spaces flag if it is necessary to process spaces (" ") as separate tokens
         * @param tabs flag if it is necessary to process tabs ("\t") as separate tokens
         * @param new_lines flag if it is necessary to process line breaks ("\n") as separate tokens
         * @param combineKeywords flag if it is necessary to combine keywords ("static", "void" -> "static void")
         * @param combineComments flag if it is necessary to combine comments ("//", "comment1" -> "// comment1")
         * @param combineTypes flag if it is necessary to combine type names ("unsigned", "long", "long" -> "unsigned long long")
         * @param combineOperators flag if it is necessary to combine operators ("+", "=" -> "+=")
         * @return std::vector of tokens
         */
        static std::vector<Token> tokenizeText(const std::string &code, bool spaces = false, bool tabs = false, bool new_lines = false, bool combine_keywords = false,
                                               bool combine_comments = false, bool combine_types = false, bool combine_operators = false);

        /*!
         * @brief makes from a text of code vector of tokens
         *
         * @param FilePath file with code to tokenize
         * @param spaces flag if it is necessary to process spaces (" ") as separate tokens
         * @param tabs flag if it is necessary to process tabs ("\t") as separate tokens
         * @param new_lines flag if it is necessary to process line breaks ("\n") as separate tokens
         * @param combine_keywords flag if it is necessary to combine keywords ("static", "void" -> "static void")
         * @param combine_comments flag if it is necessary to combine comments ("//", "comment1" -> "// comment1")
         * @param combine_types flag if it is necessary to combine type names ("unsigned", "long", "long" -> "unsigned long long")
         * @param combine_operators flag if it is necessary to combine operators ("+", "=" -> "+=")
         * @return std::vector of tokens
         */
        static std::vector<Token> tokenizeFile(const std::string &FilePath, bool spaces = false, bool tabs = false, bool new_lines = false, bool combine_keywords = false,
                                               bool combine_comments = false, bool combine_types = false, bool combine_operators = false);
    };
}

namespace Constants {
    constexpr size_t numberOfKeywords = 65, numberOfTypenames = 26, numberOfOperators = 18, numberOfSpecialYamlSymbols = 8, numberOfConfigDatatypes = 7;

    const std::array<std::string, numberOfKeywords> keywords = {"alignas", "alignof", "__asm", "break", "case", "catch", "class", "concept", "const", "constexpr",
                                                                "consteval", "constinit", "continue", "const_case", "co_await", "co_return", "co_yield", "decltype",
                                                                "default", "delete", "do", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "for",
                                                                "friend", "goto", "if", "inline", "mutable", "namespace", "new", "noexcept", "operator", "private",
                                                                "public", "protected", "register", "reinterpret_cast", "requires", "return", "sizeof", "static",
                                                                "static_assert", "static_cast", "struct", "switch", "template", "typename", "this", "thread_local",
                                                                "throw", "true", "try", "typedef", "typeid", "using", "union", "virtual", "override", "while"};

    const std::array<std::string, numberOfTypenames> typenames = {"bool", "char", "signed", "unsigned", "wchar_t", "char16_t", "char32_t", "short", "int", "long",
                                                                  "float", "double", "void", "auto", "char8_t", "nullptr", "size_t", "volatile", "int8_t", "uint8_t",
                                                                  "int16_t", "uint16_t", "int32_t", "uint32_t", "int64_t", "uint64_t"};

    const std::array<std::string, numberOfOperators> operators = {"+", "-", "=", "*", "&", "|", "^", "%", "?", ":", "=", "<", ">", "/", "!", "~", ".", ","};

    const std::array<char, numberOfSpecialYamlSymbols> specialYamlSymbols = {',', ' ', '\n', '\t', ':', '[', '-'};

    inline constexpr std::string_view config_path = "config.yml";

    inline constexpr std::string_view config_arguement_not_found_error_start = "An error was detected during the processing of the config.yaml: ";

    inline constexpr std::string_view config_arguement_not_found_error_end = " not found.\nPlease check that the following items are present in the configuration file: ";

    inline constexpr std::string_view config_quotes_error = "The number of quotation marks is odd. Please, check the correctness of entered data (paths must be enclosed in quotation marks)";

    inline constexpr std::string_view config_paths_processing_error = "Something went wrong. Please check the structure of config. Remember, all paths must be inside quote marks and enumerated in [] or with '-' on a few lines";
}

namespace Exceptions {
    /*!
     * @brief just calling an exception with the given description
     */
    class ExceptionWithDescription: public std::exception {
    private:

        std::string message;

    public:

        /*!
         * @brief constructor that helps to give more descriptive errors. Just use this after "throw" with string you want to put to console
         *
         * @param message a string to show
         */
        explicit ExceptionWithDescription(std::string message): message(std::move(message)) {}

        [[nodiscard]] char const *what() const noexcept override;

    };
}

namespace StringTools {
    /*!
     * @brief replace all occurrences of a substring in a string
     *
     * @param data a string where you want to make replacing
     * @param toSearch a string you want to replace
     * @param replaceStr a string you want to have in the place of toSearch
     */
    void replaceAll(std::string &data, const std::string &toSearch, const std::string &replaceStr);

    /*!
     * @brief function mostly for config processing - helps to find a value for a key in yaml
     *
     * @param str a string where to find word
     * @param idx an index of word the previous to that you want to get
     * @return the pair of the next value and its start index
     */
    std::pair<std::string, size_t> findNextWord(const std::string &str, const size_t &idx);
}

namespace Config {
    enum class ConfigDatatype {
        PROJECT_NAME,
        ROOT_PATH,
        LOGO_PATH,
        THEME,
        FILES_TO_PROCESS_PATHS,
        REPOSITORY_URL,
        MD_FLAG,
        UNKNOWN_DATA
    };

    constexpr std::array<ConfigDatatype, Constants::numberOfConfigDatatypes> configDatatypes = {ConfigDatatype::PROJECT_NAME, ConfigDatatype::ROOT_PATH,
                                                                                                ConfigDatatype::LOGO_PATH, ConfigDatatype::FILES_TO_PROCESS_PATHS,
                                                                                                ConfigDatatype::THEME, ConfigDatatype::REPOSITORY_URL,
                                                                                                ConfigDatatype::MD_FLAG};

    std::string configDataType2string(const ConfigDatatype &configDatatype);

    /*!
     * @brief separate processing of paths
     *
     * @param str a string where to find paths
     * @param idx an index of a key
     * @return vector of paths
     */
    std::vector<std::string> getAllEnumeratedPaths(const std::string &str, const size_t &idx);

    /*!
     * @brief gets all the values for all known keys of config file
     *
     * @return hashtable with pairs of config data types and their arguments
     */
    std::unordered_map<ConfigDatatype, std::string> processConfig();
}

#endif //CPPTESTLINTDOC_UTILS_H
