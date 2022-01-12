#include "Tokenizer.h"

std::vector<std::string> keywords = {"alignas", "alignof", "__asm", "break", "case", "catch", "class", "concept", "const", "constexpr", "consteval",
                                     "constinit", "continue", "const_case", "co_await", "co_return", "co_yield", "decltype", "default", "delete",
                                     "do", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "for", "friend", "goto", "if",
                                     "inline", "mutable", "namespace", "new", "noexcept", "operator", "private", "public", "protected", "register",
                                     "reinterpret_cast", "requires", "return", "sizeof", "static", "static_assert", "static_cast", "struct", "switch",
                                     "template", "typename", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "using", "union",
                                     "virtual", "override", "while"};

std::vector<std::string> typenames = {"bool", "char", "signed", "unsigned", "wchar_t", "char16_t", "char32_t", "short", "int", "long", "float",
                                      "double", "void", "auto", "char8_t", "nullptr", "size_t", "volatile", "int8_t", "uint8_t", "int16_t",
                                      "uint16_t", "int32_t", "uint32_t", "int64_t", "uint64_t"};

std::vector<std::string> operators = {"+", "-", "=", "*", "&", "|", "^", "%", "?", ":", "=", "<", ">", "/", "!", "~", ".", ","};

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
            token_type = TokenType::UNKNOWN;
    }
    token_value = str;
}

std::string CodeParser::Token::type()
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

void CodeParser::CombineTypes(std::vector<CodeParser::Token> &tokens)
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

void CodeParser::CombineOperators(std::vector<CodeParser::Token> &tokens)
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

std::vector<CodeParser::Token> CodeParser::TokenizeText(const std::string &code, bool spaces, bool tabs, bool new_lines)
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
                Tokens.emplace_back(Token(temp));
                temp.clear();
                continue;
            }
            else
            {
                inString = true;
                if (!temp.empty())
                    Tokens.emplace_back(Token(temp));
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
                if (!temp.empty())
                    Tokens.emplace_back(Token(temp));
                temp.clear();
                if (i == ' ' && spaces)
                    Tokens.emplace_back(Token(" "));
                else if (i == '\n' && new_lines)
                    Tokens.emplace_back(Token("\n"));
                else if (i == '\t' && tabs)
                    Tokens.emplace_back(Token("\t"));
            }
            else if (std::find(operators.begin(), operators.end(), std::to_string(i)) != operators.end() || i == '(' || i == ')' || i == '[' ||
                     i == ']' || i == '{' || i == '}')
            {
                if (!temp.empty())
                    Tokens.emplace_back(Token(temp));
                temp.clear();
                temp.push_back(i);
                if (!temp.empty())
                    Tokens.emplace_back(Token(temp));
                temp.clear();
            }
            else if (i == "'"[0])
            {
                if (!inQuote)
                {
                    inQuote = true;
                    if (!temp.empty())
                        Tokens.emplace_back(Token(temp));
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

    CombineOperators(Tokens);
    CombineTypes(Tokens);

    return Tokens;
}

std::vector<CodeParser::Token> CodeParser::TokenizeFile(const std::string &FileName, bool spaces, bool tabs, bool new_lines)
{
    std::ifstream file;
    file.open(FileName);

    std::stringstream temp;
    temp << file.rdbuf();

    std::string code = temp.str();

    return TokenizeText(code, spaces, tabs, new_lines);
}