/*!
    @author Kozulin Ilya
*/

#include "Tokenizer.h"

const std::vector<std::string> keywords = {"alignas", "alignof", "__asm", "break", "case", "catch", "class", "concept", "const", "constexpr", "consteval", "constinit",
                                           "continue", "const_case", "co_await", "co_return", "co_yield", "decltype", "default", "delete", "do", "dynamic_cast", "else",
                                           "enum", "explicit", "export", "extern", "false", "for", "friend", "goto", "if", "inline", "mutable", "namespace", "new",
                                           "noexcept", "operator", "private", "public", "protected", "register", "reinterpret_cast", "requires", "return", "sizeof",
                                           "static", "static_assert", "static_cast", "struct", "switch", "template", "typename", "this", "thread_local", "throw", "true",
                                           "try", "typedef", "typeid", "using", "union", "virtual", "override", "while"};

const std::vector<std::string> typenames = {"bool", "char", "signed", "unsigned", "wchar_t", "char16_t", "char32_t", "short", "int", "long", "float", "double", "void",
                                            "auto", "char8_t", "nullptr", "size_t", "volatile", "int8_t", "uint8_t", "int16_t", "uint16_t", "int32_t", "uint32_t",
                                            "int64_t", "uint64_t"};

const std::vector<std::string> operators = {"+", "-", "=", "*", "&", "|", "^", "%", "?", ":", "=", "<", ">", "/", "!", "~", ".", ","};

CodeParser::Token::Token()
{
    token_type = TokenType::UNKNOWN;

    token_value = "";
}

CodeParser::Token::Token(const std::string &str)
{
    if (std::find(keywords.begin(), keywords.end(), str) != keywords.end())
    {
        if (str == "class")
            token_type = TokenType::CLASS;

        else if (str == "struct")
            token_type = TokenType::STRUCT;

        else
            token_type = TokenType::KEYWORD;
    }
    else if (std::find(operators.begin(), operators.end(), str) != operators.end())
        token_type = TokenType::OPERATOR;

    else if (std::find(typenames.begin(), typenames.end(), str) != typenames.end())
        token_type = TokenType::TYPENAME;

    else if (str == "///" || str == "//" || str == "/*" || str == "*/" || str == "/*!" || str == "//!" || str == "///<")
        token_type = TokenType::COMMENT;

    else if (str.size() == 1)
    {
        if (str >= "0" && str <= "9")
            token_type = TokenType::LITERAL;

        else if (str == "(" || str == "[" || str == "{")
            token_type = TokenType::OPEN_BRACKET;

        else if (str == ")" || str == "]" || str == "}")
            token_type = TokenType::CLOSE_BRACKET;

        else if (str == ";")
            token_type = TokenType::SEMICOLON;

        else if (str == " " || str == "\t" || str == "\n")
        {
            token_type = TokenType::PASS_SYMBOL;

            if (str == " ")
                token_value = "*space*";

            else if (str == "\t")
                token_value = "*tab*";

            else
                token_value = "*new_line*";
            return;
        }
        else
            token_type = TokenType::UNKNOWN;
    }
    else
    {
        bool isDigit = true;
        for (char i : str)
        {
            if (!std::isdigit(i))
            {
                isDigit = false;
                break;
            }
        }
        if (isDigit)
            token_type = TokenType::LITERAL;
        else
        {
            if (str[0] == '"' && str[str.size()-1] == '"' || str[0] == "'"[0] && str[str.size()-1] == "'"[0])
                token_type = TokenType::STRING;
            else
                token_type = TokenType::UNKNOWN;
        }
    }
    token_value = str;
}

std::string CodeParser::Token::type() const
{
    switch (token_type)
    {
        case TokenType::TYPENAME:
            return "typename";

        case TokenType::LITERAL:
            return "literal";

        case TokenType::KEYWORD:
            return "keyword";

        case TokenType::STRING:
            return "string";

        case TokenType::CLASS:
            return "class";

        case TokenType::STRUCT:
            return "struct";

        case TokenType::FUNCTION:
            return "function";

        case TokenType::VARIABLE:
            return "variable";

        case TokenType::OPERATOR:
            return "operator";

        case TokenType::SEMICOLON:
            return "semicolon";

        case TokenType::OPEN_BRACKET:
            return "open bracket";

        case TokenType::CLOSE_BRACKET:
            return "close bracket";

        case TokenType::PASS_SYMBOL:
            return "pass symbol";

        case TokenType::COMMENT:
            return "comment";

        case TokenType::UNION:
            return "union";

        case TokenType::ENUM:
            return "enum";

        case TokenType::ALIAS:
            return "alias";

        case TokenType::USER_TYPE:
            return "usertype";

        default:
            return "unknown";
    }
}

std::string CodeParser::Token::getToken() const
{
    return token_value;
}

CodeParser::TokenType CodeParser::Token::getType() const
{
    return token_type;
}

void CodeParser::Token::setToken(const std::string &str)
{
    token_value = str;
}

void CodeParser::Token::setType(const CodeParser::TokenType &_type)
{
    token_type = _type;
}

std::vector<std::string> CodeParser::Token::findUserTypes(const std::vector<Token> &tokens)
{
    std::vector<std::string> user_types;

    for (int i = 1; i < tokens.size(); i++)
    {
        if ((tokens[i - 1].getType() == TokenType::CLASS || tokens[i - 1].getType() == TokenType::STRUCT || tokens[i - 1].getType() == TokenType::UNION ||
             tokens[i - 1].getType() == TokenType::ENUM || tokens[i - 1].getToken() == "enum class") && tokens[i].getType() == TokenType::UNKNOWN)
            user_types.emplace_back(tokens[i].getToken());
    }

    return user_types;
}

void CodeParser::Token::setUserTokenTypes(std::vector<Token> &tokens, const std::vector<std::string> &user_types)
{
    for (auto &token : tokens)
        if (std::find(user_types.begin(), user_types.end(), token.getToken()) != user_types.end() && token.getType() == TokenType::UNKNOWN)
            token.setType(TokenType::USER_TYPE);
}

CodeParser::Token CodeParser::Token::joinTokens(const CodeParser::Token &token1, const CodeParser::Token &token2, bool space)
{
    if (token1.getType() != token2.getType())
        return Token();
    Token _token;
    if (space)
        _token.setToken(token1.getToken() + " " + token2.getToken());
    else
        _token.setToken(token1.getToken() + token2.getToken());

    _token.setType(token1.getType());

    return _token;
}

void CodeParser::Token::changeAliasAndMacroTokens(std::vector<CodeParser::Token> &tokens)
{
    std::unordered_map<std::string, TokenType> macro, alias;

    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].getToken() == "#define" && i < tokens.size() - 2 && tokens[i + 2].getToken() != "*new_line*")
        {
            Token temp = tokens[i + 2];
            for (int j = i + 3; j < tokens.size(); j++)
            {
                if (tokens[j].getToken() == "*new_line*")
                    break;
                temp = Token().joinTokens(temp, tokens[j], true);
            }
            macro[tokens[i + 1].getToken()] = temp.getType();
            i += 3;
        }
    }

    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].getToken() == "typedef")
        {
            for (int j = i + 1; j < tokens.size(); j++)
            {
                if (tokens[j].getType() == TokenType::SEMICOLON)
                {
                    alias[tokens[j - 1].getToken()] = tokens[i + 1].getType();
                    break;
                }
            }
        }
        else if (tokens[i].getToken() == "using" && tokens[i + 2].getToken() == "=")
            alias[tokens[i + 1].getToken()] = tokens[i + 3].getType();
    }

    for (auto &token: tokens)
    {
        if (macro.find(token.getToken()) != macro.end())
            token.setType(macro[token.getToken()]);
        else if (alias.find(token.getToken()) != alias.end())
            token.setType(alias[token.getToken()]);
    }
}

void CodeParser::Token::CombineKeywords(std::vector<CodeParser::Token> &tokens)
{
    for (int i = 1; i < tokens.size(); i++)
    {
        if (tokens[i].getType() == CodeParser::TokenType::KEYWORD && tokens[i - 1].getType() == CodeParser::TokenType::KEYWORD)
        {

            tokens[i - 1].setToken(tokens[i - 1].getToken() + " " + tokens[i].getToken());
            tokens.erase(tokens.begin() + i);
            i--;
        }
    }
}

void CodeParser::Token::CombineComments(std::vector<CodeParser::Token> &tokens)
{
    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].getType() == CodeParser::TokenType::COMMENT)
        {
            if (tokens[i].getToken() == "///" || tokens[i].getToken() == "//" || tokens[i].getToken() == "///<" || tokens[i].getToken() == "//!")
            {
                for (int j = i + 1; j < tokens.size(); j++)
                {
                    if (tokens[j].getToken() == "*new_line*")
                    {
                        i = j;
                        break;
                    }
                    tokens[j].setType(CodeParser::TokenType::COMMENT);
                }
            }
            else
            {
                for (int j = i + 1; j < tokens.size(); j++)
                {
                    if (tokens[j].getToken() == "*/")
                    {
                        i = j;
                        break;
                    }
                    tokens[j].setType(CodeParser::TokenType::COMMENT);
                }
            }
        }
    }
    for (int i = 1; i < tokens.size(); i++)
    {
        if (tokens[i].getType() == CodeParser::TokenType::COMMENT && tokens[i - 1].getType() == CodeParser::TokenType::COMMENT)
        {
            tokens[i - 1].setToken(tokens[i - 1].getToken() + " " + tokens[i].getToken());
            tokens.erase(tokens.begin() + i);
            i--;
        }
    }
}

void CodeParser::Token::CombineTypes(std::vector<CodeParser::Token> &tokens)
{
    for (int i = 1; i < tokens.size(); i++)
    {
        if (tokens[i].getType() == CodeParser::TokenType::TYPENAME && tokens[i - 1].getType() == CodeParser::TokenType::TYPENAME)
        {
            tokens[i - 1].setToken(tokens[i - 1].getToken() + " " + tokens[i].getToken());
            tokens.erase(tokens.begin() + i);
            i--;
        }
    }
}

void CodeParser::Token::CombineOperators(std::vector<CodeParser::Token> &tokens)
{
    for (int i = 1; i < tokens.size(); i++)
    {
        if (tokens[i].getType() == CodeParser::TokenType::OPERATOR && tokens[i - 1].getType() == CodeParser::TokenType::OPERATOR)
        {
            tokens[i - 1].setToken(tokens[i - 1].getToken() + tokens[i].getToken());
            tokens.erase(tokens.begin() + i);
            i--;
        }
    }
}

void CodeParser::Token::AddToken(std::vector<CodeParser::Token> &tokens, const std::string &token)
{
    if (!token.empty())
    {
        if (token.find(';') != std::string::npos && token.size() > 1)
        {
            std::string str1 = token.substr(0, token.size() - 1), str2 = ";";
            tokens.emplace_back(Token(str1));
            tokens.emplace_back(Token(str2));
        }
        else
            tokens.emplace_back(Token(token));
    }
}

std::vector<CodeParser::Token>
CodeParser::Token::TokenizeText(const std::string &code, bool spaces, bool tabs, bool new_lines, bool combineKeywords, bool combineComments, bool combineTypes,
                                bool combineOperators)
{
    std::vector<CodeParser::Token> Tokens;

    bool inString = false, inQuote = false;
    std::string temp;
    for (char i : code)
    {
        if (i == '"')
        {
            if (inString)
            {
                inString = false;
                temp.push_back(i);
                AddToken(Tokens, temp);
                temp.clear();
                continue;
            }
            else
            {
                inString = true;
                AddToken(Tokens, temp);
                temp.clear();
                temp.push_back(i);
                continue;
            }
        }
        if (inString)
        {
            temp.push_back(i);
        }
        else
        {
            if (i == ' ' || i == '\n' || i == '\t')
            {
                AddToken(Tokens, temp);
                temp.clear();
                if (i == ' ' && spaces)
                    Tokens.emplace_back(Token(" "));
                else if (i == '\n' && new_lines)
                    Tokens.emplace_back(Token("\n"));
                else if (i == '\t' && tabs)
                    Tokens.emplace_back(Token("\t"));
            }
            else if (std::find(operators.begin(), operators.end(), std::to_string(i)) != operators.end() || i == '(' || i == ')' || i == '[' || i == ']' || i == '{' ||
                     i == '}')
            {
                if (!temp.empty())
                {
                    AddToken(Tokens, temp);
                    temp.clear();
                    temp.push_back(i);
                    AddToken(Tokens, temp);
                    temp.clear();
                }
            }
            else if (i == "'"[0])
            {
                if (!inQuote)
                {
                    inQuote = true;
                    AddToken(Tokens, temp);
                    temp.clear();
                    temp.push_back(i);
                }
                else
                {
                    inQuote = false;
                    temp.push_back(i);
                    Tokens.emplace_back(Token(temp));
                    temp.clear();
                }
            }
            else
            {
                temp.push_back(i);
            }
        }
    }

    Tokens.emplace_back(Token(temp));

    if (combineComments)
        CombineComments(Tokens);

    changeAliasAndMacroTokens(Tokens);

    if (combineKeywords)
        CombineKeywords(Tokens);
    if (combineOperators)
        CombineOperators(Tokens);
    if (combineTypes)
        CombineTypes(Tokens);

    setUserTokenTypes(Tokens, findUserTypes(Tokens));

    return Tokens;
}

std::vector<CodeParser::Token>
CodeParser::Token::TokenizeFile(const std::string &FileName, bool spaces, bool tabs, bool new_lines, bool combineKeywords, bool combineComments, bool combineTypes,
                                bool combineOperators)
{
    std::ifstream file;
    file.open(FileName);

    std::stringstream temp;
    temp << file.rdbuf();

    std::string code = temp.str();

    return TokenizeText(code, spaces, tabs, new_lines, combineKeywords, combineComments, combineTypes, combineOperators);
}
