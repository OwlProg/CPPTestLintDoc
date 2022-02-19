/*!
    @author Ailurus
*/


#include "Utils.h"

CodeParser::Token::Token() {
    token_type = TokenType::UNKNOWN;
}

CodeParser::Token::Token(const std::string &str) {
    if (std::find(Constants::keywords.begin(), Constants::keywords.end(), str) != Constants::keywords.end()) {
        if (str == "class") {
            token_type = TokenType::CLASS_DEF;
        }

        else if (str == "struct") {
            token_type = TokenType::STRUCT_DEF;
        }

        else {
            token_type = TokenType::KEYWORD;
        }
    }
    else if (str[0] == '#' && str.size() > 1) {
        token_type = TokenType::PREPROCESSOR_DIRECTIVE;
    }

    else if (std::find(Constants::operators.begin(), Constants::operators.end(), str) != Constants::operators.end()) {
        token_type = TokenType::OPERATOR;
    }

    else if (std::find(Constants::typenames.begin(), Constants::typenames.end(), str) != Constants::typenames.end()) {
        token_type = TokenType::TYPENAME;
    }

    else if (str == "///" || str == "//" || str == "/*" || str == "*/" || str == "/*!" || str == "//!" || str == "///<") {
        token_type = TokenType::COMMENT;
    }

    else if (str.size() == 1) {
        if (str >= "0" && str <= "9") {
            token_type = TokenType::LITERAL;
        }

        else if (str == "(" || str == "[" || str == "{") {
            token_type = TokenType::OPEN_BRACKET;
        }

        else if (str == ")" || str == "]" || str == "}") {
            token_type = TokenType::CLOSE_BRACKET;
        }

        else if (str == ";") {
            token_type = TokenType::SEMICOLON;
        }

        else if (str == " " || str == "\t" || str == "\n") {
            token_type = TokenType::PASS_SYMBOL;

            if (str == " ") {
                token_value = "*space*";
            }

            else if (str == "\t") {
                token_value = "*tab*";
            }

            else {
                token_value = "*new_line*";
            }
            return;
        }
        else {
            token_type = TokenType::UNKNOWN;
        }
    }
    else {
        bool isDigit = true;
        for (char i : str) {
            if (!std::isdigit(i)) {
                isDigit = false;
                break;
            }
        }
        if (isDigit) {
            token_type = TokenType::LITERAL;
        }
        else {
            if (str[0] == '"' && str[str.size() - 1] == '"' || str[0] == "'"[0] && str[str.size() - 1] == "'"[0]) {
                token_type = TokenType::STRING;
            }
            else {
                token_type = TokenType::UNKNOWN;
            }
        }
    }
    token_value = str;
}

std::string CodeParser::type2string(const CodeParser::TokenType &tokenType) {
    switch (tokenType) {
        case CodeParser::TokenType::TYPENAME:
            return "typename";

        case CodeParser::TokenType::LITERAL:
            return "literal";

        case CodeParser::TokenType::KEYWORD:
            return "keyword";

        case CodeParser::TokenType::STRING:
            return "string";

        case CodeParser::TokenType::CLASS_DEF:
            return "class";

        case CodeParser::TokenType::STRUCT_DEF:
            return "struct";

        case CodeParser::TokenType::FUNCTION_NAME:
            return "function";

        case CodeParser::TokenType::VARIABLE_NAME:
            return "variable";

        case CodeParser::TokenType::OPERATOR:
            return "operator";

        case CodeParser::TokenType::SEMICOLON:
            return "semicolon";

        case CodeParser::TokenType::OPEN_BRACKET:
            return "open bracket";

        case CodeParser::TokenType::CLOSE_BRACKET:
            return "close bracket";

        case CodeParser::TokenType::PASS_SYMBOL:
            return "pass symbol";

        case CodeParser::TokenType::COMMENT:
            return "comment";

        case CodeParser::TokenType::UNION_DEF:
            return "union";

        case CodeParser::TokenType::ENUM_DEF:
            return "enum";

        case CodeParser::TokenType::ALIAS:
            return "alias";

        case CodeParser::TokenType::PREPROCESSOR_DIRECTIVE:
            return "preprocessor directive";

        default:
            return "unknown";
    }
}

std::string CodeParser::Token::getToken() const {
    return token_value;
}

CodeParser::TokenType CodeParser::Token::getType() const {
    return token_type;
}

void CodeParser::Token::setToken(const std::string &str) {
    token_value = str;
}

void CodeParser::Token::setType(const CodeParser::TokenType &type) {
    token_type = type;
}

std::vector<std::string> CodeParser::Token::findUserTypes(const std::vector<Token> &tokens) {
    std::vector<std::string> user_types;

    for (int i = 1; i < tokens.size(); i++) {
        if ((tokens[i - 1].getType() == TokenType::CLASS_DEF || tokens[i - 1].getType() == TokenType::STRUCT_DEF || tokens[i - 1].getType() == TokenType::UNION_DEF ||
             tokens[i - 1].getType() == TokenType::ENUM_DEF || tokens[i - 1].getToken() == "enum class") && tokens[i].getType() == TokenType::UNKNOWN) {
            user_types.emplace_back(tokens[i].getToken());
        }
    }

    return user_types;
}

void CodeParser::Token::setUserTokenTypes(std::vector<Token> &tokens, const std::vector<std::string> &user_types) {
    for (auto &token : tokens)
        if (std::find(user_types.begin(), user_types.end(), token.getToken()) != user_types.end() && token.getType() == TokenType::UNKNOWN) {
            token.setType(TokenType::TYPENAME);
        }
}

CodeParser::Token CodeParser::Token::joinTokens(const CodeParser::Token &token1, const CodeParser::Token &token2, bool space) {
    if (token1.getType() != token2.getType()) {
        return Token();
    }
    Token token_;
    if (space) {
        token_.setToken(token1.getToken() + " " + token2.getToken());
    }
    else {
        token_.setToken(token1.getToken() + token2.getToken());
    }

    token_.setType(token1.getType());

    return token_;
}

void CodeParser::Token::changeAliasAndMacroTokens(std::vector<CodeParser::Token> &tokens) {
    std::unordered_map<std::string, TokenType> macro, alias;

    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].getToken() == "#define" && i < tokens.size() - 2 && tokens[i + 2].getToken() != "*new_line*") {
            Token temp = tokens[i + 2];
            for (int j = i + 3; j < tokens.size(); j++) {
                if (tokens[j].getToken() == "*new_line*") {
                    break;
                }
                temp = Token().joinTokens(temp, tokens[j], true);
            }
            macro[tokens[i + 1].getToken()] = temp.getType();
            i += 3;
        }
    }

    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].getToken() == "typedef") {
            for (int j = i + 1; j < tokens.size(); j++) {
                if (tokens[j].getType() == TokenType::SEMICOLON) {
                    alias[tokens[j - 1].getToken()] = tokens[i + 1].getType();
                    break;
                }
            }
        }
        else if (tokens[i].getToken() == "using" && tokens[i + 2].getToken() == "=") {
            alias[tokens[i + 1].getToken()] = tokens[i + 3].getType();
        }
    }

    for (auto &token: tokens) {
        if (macro.find(token.getToken()) != macro.end()) {
            token.setType(macro[token.getToken()]);
        }
        else if (alias.find(token.getToken()) != alias.end()) {
            token.setType(alias[token.getToken()]);
        }
    }
}

void CodeParser::Token::combineKeywords(std::vector<CodeParser::Token> &tokens) {
    for (int i = 1; i < tokens.size(); i++) {
        if (tokens[i].getType() == CodeParser::TokenType::KEYWORD && tokens[i - 1].getType() == CodeParser::TokenType::KEYWORD) {

            tokens[i - 1].setToken(tokens[i - 1].getToken() + " " + tokens[i].getToken());
            tokens.erase(tokens.begin() + i);
            i--;
        }
    }
}

void CodeParser::Token::combineComments(std::vector<CodeParser::Token> &tokens) {
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].getType() == CodeParser::TokenType::COMMENT) {
            if (tokens[i].getToken() == "///" || tokens[i].getToken() == "//" || tokens[i].getToken() == "///<" || tokens[i].getToken() == "//!") {
                for (int j = i + 1; j < tokens.size(); j++) {
                    if (tokens[j].getToken() == "*new_line*") {
                        i = j;
                        break;
                    }
                    tokens[j].setType(CodeParser::TokenType::COMMENT);
                }
            }
            else {
                for (int j = i + 1; j < tokens.size(); j++) {
                    if (tokens[j].getToken() == "*/") {
                        i = j;
                        break;
                    }
                    tokens[j].setType(CodeParser::TokenType::COMMENT);
                }
            }
        }
    }
    for (int i = 1; i < tokens.size(); i++) {
        if (tokens[i].getType() == CodeParser::TokenType::COMMENT && tokens[i - 1].getType() == CodeParser::TokenType::COMMENT) {
            tokens[i - 1].setToken(tokens[i - 1].getToken() + " " + tokens[i].getToken());
            tokens.erase(tokens.begin() + i);
            i--;
        }
    }
}

void CodeParser::Token::combineTypes(std::vector<CodeParser::Token> &tokens) {
    for (int i = 1; i < tokens.size(); i++) {
        if (tokens[i].getType() == CodeParser::TokenType::TYPENAME && tokens[i - 1].getType() == CodeParser::TokenType::TYPENAME) {
            tokens[i - 1].setToken(tokens[i - 1].getToken() + " " + tokens[i].getToken());
            tokens.erase(tokens.begin() + i);
            i--;
        }
    }
}

void CodeParser::Token::combineOperators(std::vector<CodeParser::Token> &tokens) {
    for (int i = 1; i < tokens.size(); i++) {
        if (tokens[i].getType() == CodeParser::TokenType::OPERATOR && tokens[i - 1].getType() == CodeParser::TokenType::OPERATOR) {
            tokens[i - 1].setToken(tokens[i - 1].getToken() + tokens[i].getToken());
            tokens.erase(tokens.begin() + i);
            i--;
        }
    }
}

void CodeParser::Token::addToken(std::vector<CodeParser::Token> &tokens, const std::string &token) {
    if (!token.empty()) {
        if (token.find(';') != std::string::npos && token.size() > 1) {
            std::string str1 = token.substr(0, token.size() - 1), str2 = ";";
            addToken(tokens, str1);
            tokens.emplace_back(Token(str2));
        }
        else if (token == "//" || token == "/*" || token == "*/" || token == "///" || token == "///<" || token == "//!" || token == "/*!") {
            tokens.emplace_back(Token(token));
        }
        else {
            size_t operator_idx = -1;
            for (size_t i = 0; i < token.size(); i++) {
                if (std::find(Constants::operators.begin(), Constants::operators.end(), std::string(1, token[i])) != Constants::operators.end()) {
                    operator_idx = i;
                    break;
                }
            }
            if (operator_idx != -1) {
                std::string before_operator = token.substr(0, operator_idx), after_operator = token.substr(operator_idx + 1, token.size() - operator_idx + 1);
                if (!before_operator.empty()) {
                    tokens.emplace_back(Token(before_operator));
                }
                tokens.emplace_back(Token(std::string(1, token[operator_idx])));
                if (!after_operator.empty()) {
                    addToken(tokens, after_operator);
                }
            }
            else {
                tokens.emplace_back(Token(token));
            }
        }
    }
    else {
        return;
    }
}

void CodeParser::Token::setVariablesAndFunctionsTokenTypes(std::vector<Token> &tokens) {
    std::unordered_set<std::string> variables, functions;

    for (size_t i = 0; i < tokens.size() - 2; i++) {
        if (tokens[i].getType() == TokenType::TYPENAME && tokens[i + 1].getType() == TokenType::UNKNOWN) {
            if (tokens[i + 2].getType() == TokenType::OPEN_BRACKET) {
                tokens[i + 1].setType(TokenType::FUNCTION_NAME);
                functions.insert(tokens[i + 1].getToken());
            }
            else {
                tokens[i + 1].setType(TokenType::VARIABLE_NAME);
                variables.insert(tokens[i + 1].getToken());
            }
        }
        else if (tokens[i].getType() == TokenType::UNKNOWN) {
            if (variables.find(tokens[i].getToken()) != variables.end()) {
                tokens[i].setType(TokenType::VARIABLE_NAME);
            }
            else if (functions.find(tokens[i].getToken()) != functions.end()) {
                tokens[i].setType(TokenType::FUNCTION_NAME);
            }
        }
    }
}

void CodeParser::Token::setClassNameTokenTypes(std::vector<Token> &tokens) {
    std::unordered_set<std::string> class_names;

    for (size_t i = 0; i < tokens.size() - 3; i++) {
        if (std::find(class_names.begin(), class_names.end(), tokens[i].getToken()) != class_names.end()) {
            tokens[i].setType(TokenType::TYPENAME);
        }
        else if (tokens[i].getType() == TokenType::UNKNOWN && tokens[i + 1].getToken() == "<" && tokens[i + 2].getType() == TokenType::TYPENAME &&
                 tokens[i + 3].getToken() == ">") {
            tokens[i].setType(TokenType::TYPENAME);
            class_names.insert(tokens[i].getToken());
        }
    }

    for (size_t i = 0; i < tokens.size() - 3; i++) {
        if (std::find(class_names.begin(), class_names.end(), tokens[i].getToken()) != class_names.end() && tokens[i + 1].getToken() == "<" &&
            tokens[i + 2].getType() == TokenType::TYPENAME && tokens[i + 3].getToken() == ">") {
            tokens[i].setToken(tokens[i].getToken() + tokens[i + 1].getToken() + tokens[i + 2].getToken() + tokens[i + 3].getToken());
            tokens[i].setType(TokenType::TYPENAME);
            tokens.erase(tokens.begin() + i + 1, tokens.begin() + i + 4);
        }
    }
}


std::vector<CodeParser::Token>
CodeParser::Token::tokenizeText(const std::string &code, bool spaces, bool tabs, bool new_lines, bool combine_keywords, bool combine_comments, bool combine_types,
                                bool combine_operators) {
    std::vector<CodeParser::Token> tokens;

    bool inString = false, inQuote = false;
    std::string temp;
    for (char i : code) {
        if (i == '"') {
            if (inString) {
                inString = false;
                temp.push_back(i);
                addToken(tokens, temp);
                temp.clear();
                continue;
            }
            else {
                inString = true;
                addToken(tokens, temp);
                temp.clear();
                temp.push_back(i);
                continue;
            }
        }
        if (inString) {
            temp.push_back(i);
        }
        else {
            if (i == ' ' || i == '\n' || i == '\t') {
                addToken(tokens, temp);
                temp.clear();
                if (i == ' ' && spaces) {
                    tokens.emplace_back(Token(" "));
                }
                else if (i == '\n' && new_lines) {
                    tokens.emplace_back(Token("\n"));
                }
                else if (i == '\t' && tabs) {
                    tokens.emplace_back(Token("\t"));
                }
            }
            else if (std::find(Constants::operators.begin(), Constants::operators.end(), std::to_string(i)) != Constants::operators.end() || i == '(' || i == ')' ||
                     i == '[' || i == ']' || i == '{' || i == '}') {
                if (!temp.empty()) {
                    addToken(tokens, temp);
                    temp.clear();
                    temp.push_back(i);
                    addToken(tokens, temp);
                    temp.clear();
                }
                else {
                    temp.push_back(i);
                    addToken(tokens, temp);
                    temp.clear();
                }
            }
            else if (i == "'"[0]) {
                if (!inQuote) {
                    inQuote = true;
                    addToken(tokens, temp);
                    temp.clear();
                    temp.push_back(i);
                }
                else {
                    inQuote = false;
                    temp.push_back(i);
                    tokens.emplace_back(Token(temp));
                    temp.clear();
                }
            }
            else {
                temp.push_back(i);
            }
        }
    }

    tokens.emplace_back(Token(temp));

    if (combine_comments) {
        combineComments(tokens);
    }

    changeAliasAndMacroTokens(tokens);

    if (combine_keywords) {
        combineKeywords(tokens);
    }
    if (combine_operators) {
        combineOperators(tokens);
    }
    if (combine_types) {
        combineTypes(tokens);
    }

    setUserTokenTypes(tokens, findUserTypes(tokens));
    setClassNameTokenTypes(tokens);
    setVariablesAndFunctionsTokenTypes(tokens);

    return tokens;
}

std::vector<CodeParser::Token>
CodeParser::Token::tokenizeFile(const std::string &FilePath, bool spaces, bool tabs, bool new_lines, bool combine_keywords, bool combine_comments, bool combine_types,
                                bool combine_operators) {
    std::ifstream file;
    file.open(FilePath);

    std::stringstream temp;
    temp << file.rdbuf();

    std::string code = temp.str();

    return tokenizeText(code, spaces, tabs, new_lines, combine_keywords, combine_comments, combine_types, combine_operators);
}

char const *Exceptions::ExceptionWithDescription::what() const noexcept {
    return message.c_str();
}

void StringTools::replaceAll(std::string &data, const std::string &toSearch, const std::string &replaceStr) {
    size_t pos = data.find(toSearch);
    while (pos != std::string::npos) {
        data.replace(pos, toSearch.size(), replaceStr);
        pos = data.find(toSearch, pos + replaceStr.size());
    }
}

std::pair<std::string, size_t> StringTools::findNextWord(const std::string &str, const size_t &idx) {
    size_t stringSize = str.size(), firstWordEnd = idx, newWordStart = idx;
    std::pair<std::string, size_t> nextWord;
    bool firstWordEnded = false, secondWordStarted = false;
    for (size_t i = idx; i < stringSize; i++) {
        bool flag = false;
        for (size_t j = 0; j < Constants::numberOfSpecialYamlSymbols; j++) {
            if (str[i] == Constants::specialYamlSymbols[j]) {
                flag = true;
                if (!firstWordEnded) {
                    firstWordEnded = true;
                    break;
                }
                if (secondWordStarted) {
                    return nextWord;
                }
            }
        }
        if (!flag) {
            if (firstWordEnded && !secondWordStarted) {
                nextWord.second = i;
                secondWordStarted = true;
            }
            if (secondWordStarted) {
                nextWord.first.push_back(str[i]);
            }
        }
    }
    return nextWord;
}

std::string Config::configDataType2string(const Config::ConfigDatatype &configDatatype) {
    switch (configDatatype) {
        case ConfigDatatype::PROJECT_NAME:
            return "ProjectName";

        case ConfigDatatype::ROOT_PATH:
            return "RootPath";

        case ConfigDatatype::LOGO_PATH:
            return "LogoPath";

        case ConfigDatatype::THEME:
            return "Theme";

        case ConfigDatatype::FILES_TO_PROCESS_PATHS:
            return "FilesToProcessPaths";

        case ConfigDatatype::REPOSITORY_URL:
            return "RepositoryURL";

        case ConfigDatatype::MD_FLAG:
            return "Markdown";

        default:
            return "Unknown";
    }
}

std::vector<std::string> Config::getAllEnumeratedPaths(const std::string &str, const size_t &idx) {
    size_t stringSize = str.size();
    int endOfPathEnumeration = -1;
    std::vector<std::string> paths;
    bool insideQuotes = false, enumViaBrackets = false;
    for (size_t i = idx; i < stringSize; i++) {
        if (str[i] == '[') {
            enumViaBrackets = true;
            break;
        }
        else if (str[i] == '-') {
            break;
        }
    }
    if (enumViaBrackets) {
        for (size_t i = idx; i < stringSize; i++) {
            if (str[i] == ']') {
                endOfPathEnumeration = i;
                break;
            }
        }
    }
    else {
        int lastNewLineIndex = -1, firstLineOfEnumeration = -1;
        bool dashFound = false;
        for (int i = idx; i < stringSize; i++) {
            if (str[i] == '\n') {
                firstLineOfEnumeration = i + 1;
                break;
            }
        }
        for (int i = firstLineOfEnumeration; i < stringSize; i++) {
            if (str[i] == '-' && !dashFound) {
                dashFound = true;
                continue;
            }
            if (str[i] == '\n' && dashFound) {
                dashFound = false;
                continue;
            }
            if (str[i] == '\n' && !dashFound) {
                endOfPathEnumeration = i;
            }
        }
        if (dashFound && endOfPathEnumeration == -1) {
            endOfPathEnumeration = stringSize;
        }
    }
    std::string curPath;
    if (endOfPathEnumeration == -1) {
        throw Exceptions::ExceptionWithDescription(std::string(Constants::config_paths_processing_error));
    }
    else {
        for (size_t i = idx; i < endOfPathEnumeration; i++) {
            if (str[i] == '"') {
                if (!insideQuotes) {
                    insideQuotes = true;
                }
                else {
                    insideQuotes = false;
                    paths.emplace_back(curPath);
                    curPath.clear();
                }
            }
            else {
                if (insideQuotes) {
                    curPath.push_back(str[i]);
                }
                else {
                    continue;
                }
            }
        }
        if (!insideQuotes) {
            return paths;
        }
        else {
            throw Exceptions::ExceptionWithDescription(std::string(Constants::config_quotes_error));
        }
    }
}

std::unordered_map<Config::ConfigDatatype, std::string> Config::processConfig() {

    std::unordered_map<Config::ConfigDatatype, std::string> config;
    std::ifstream file;
    std::string path = std::string(Constants::config_path);
    file.open(std::string(Constants::config_path), std::ifstream::in);

    std::stringstream temp;
    temp << file.rdbuf();

    std::string configContent = temp.str();
    size_t configSize = configContent.size();
    std::string allConfigDatatypes;
    for (const Config::ConfigDatatype &datatype: Config::configDatatypes) {
        allConfigDatatypes.append(configDataType2string(datatype) + ", ");
    }

    for (const Config::ConfigDatatype &datatype: Config::configDatatypes) {
        size_t idx = configContent.find(configDataType2string(datatype));

        if (idx != std::string::npos) {
            if (datatype != Config::ConfigDatatype::FILES_TO_PROCESS_PATHS) {
                config[datatype] = (StringTools::findNextWord(configContent, idx)).first;
                if (config[datatype][0] == '"' && config[datatype][config[datatype].size() - 1] == '"') {
                    config[datatype].pop_back(), config[datatype].erase(0, 1);
                }
            }
            else {
                std::vector<std::string> paths = getAllEnumeratedPaths(configContent, idx);
                for (const auto &it: paths) {
                    config[datatype].append(it + ' ');
                }
            }
        }
        else {
            throw Exceptions::ExceptionWithDescription(std::string(Constants::config_arguement_not_found_error_start) + configDataType2string(datatype) +
                                                       std::string(Constants::config_arguement_not_found_error_end) + allConfigDatatypes);
        }
    }

    return config;
}