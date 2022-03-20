/*!
 *  @author Ailurus
 */

#include "AutoDoc.h"

#pragma region ObjectInfo

DocGen::ObjectInfo::ObjectInfo() {
    type = ObjectType::UNKNOWN;
}

void DocGen::ObjectInfo::setType(const ObjectType &_type) {
    type = _type;
}

void DocGen::ObjectInfo::setInfo(const InfoType &_type, const std::string &_doc) {
    info.insert(std::make_pair(_type, _doc));
}

std::multimap<DocGen::InfoType, std::string> DocGen::ObjectInfo::getInfo() const {
    return info;
}

std::string DocGen::objecttype2string(const DocGen::ObjectType &objectType) {
    switch (objectType) {

        case ObjectType::USERTYPE:
            return "Usertype";

        case ObjectType::GLOBAL_VARIABLE_OR_FUNCTION:
            return "Global Variable Or Function";

        default:
            return "Unknown";
    }
}

std::string DocGen::infotype2string(const DocGen::InfoType &infoType) {
    switch (infoType) {
        case InfoType::TYPE_OF_OBJECT:
            return "Type";

        case InfoType::SHORT_NAME:
            return "Name";

        case InfoType::FULL_NAME:
            return "Initialization";

        case InfoType::NAMESPACE:
            return "Namespace";

        case InfoType::BRIEF:
            return "Brief";

        case InfoType::FILE:
            return "File";

        case InfoType::MEMBER:
            return "Member";

        default:
            return "Unknown";
    }
}

std::string DocGen::ObjectInfo::commentPreprocessing(std::string str) {
    std::vector<std::string> symbols_to_delete = {"@", "//", " * ", "/*!", "*/", "/*"};
    for (const auto &it: symbols_to_delete)
        StringTools::replaceAll(str, it, "");
    StringTools::replaceAll(str, "*new_line*", "\n");
    return str;
}

DocGen::ObjectInfo::ObjectInfo(const std::vector<CodeParser::Token> &code, const size_t &object_idx) {
    if (code[object_idx].getType() == CodeParser::TokenType::TYPENAME && code[object_idx + 1].getType() == CodeParser::TokenType::VARIABLE_NAME) {
        bool infoAdded = false;
        if (object_idx > 2 && code[object_idx - 2].getType() == CodeParser::TokenType::COMMENT) {
            info.insert(std::make_pair(InfoType::BRIEF, commentPreprocessing(code[object_idx - 2].getToken())));
            infoAdded = true;
        }
        else {
            std::string comment;
            for (size_t idx = object_idx; idx < code.size(); idx++) {
                if (code[idx].getToken() == "*new_line*") {
                    break;
                }
                if (code[idx].getType() == CodeParser::TokenType::COMMENT) {
                    comment.append(code[idx].getToken());
                }
            }
            if (!comment.empty()) {
                info.insert(std::make_pair(InfoType::BRIEF, commentPreprocessing(comment)));
                infoAdded = true;
            }
        }
        if (infoAdded) {
            info.insert(std::make_pair(InfoType::TYPE_OF_OBJECT, code[object_idx].getToken()));
            std::string init;
            size_t idx = object_idx;
            while (code[idx - 1].getToken() != "*new_line*") {
                idx--;
            }
            while (code[idx].getToken() != "*new_line*") {
                if (code[idx].getType() != CodeParser::TokenType::COMMENT) {
                    if (code[idx].getToken() != "{" && code[idx].getToken() != "}" && code[idx].getToken() != "." && code[idx - 1].getToken() != "." &&
                        code[idx - 1].getToken() != "{" && code[idx].getToken() != ";") {
                        init.append(" ");
                    }
                    init.append(code[idx].getToken());
                }
                idx++;
            }
            info.insert(std::make_pair(InfoType::FULL_NAME, init));
            info.insert(std::make_pair(InfoType::SHORT_NAME, code[object_idx + 1].getToken()));
        }
    }
    else if (code[object_idx].getType() == CodeParser::TokenType::TYPENAME && code[object_idx + 1].getType() == CodeParser::TokenType::FUNCTION_NAME) {
        bool infoAdded = false;
        if (object_idx > 2 && code[object_idx - 2].getType() == CodeParser::TokenType::COMMENT) {
            info.insert(std::make_pair(InfoType::BRIEF, commentPreprocessing(code[object_idx - 2].getToken())));
            infoAdded = true;
        }
        else {
            for (size_t j = object_idx; j < code.size(); j++) {
                if (code[j].getToken() == "*new_line*" && code[j - 1].getType() == CodeParser::TokenType::COMMENT) {
                    info.insert(std::make_pair(InfoType::BRIEF, commentPreprocessing(code[j - 1].getToken())));
                    infoAdded = true;
                }
                else if (code[j].getToken() == "*new_line*" && code[j - 1].getType() != CodeParser::TokenType::COMMENT) {
                    break;
                }
            }
        }
        if (infoAdded) {
            info.insert(std::make_pair(InfoType::TYPE_OF_OBJECT, code[object_idx].getToken()));
            std::string init;
            size_t idx = object_idx;
            while (code[idx - 1].getToken() != "*new_line*") {
                idx--;
            }
            while (code[idx].getToken() != "*new_line*") {
                if (code[idx].getType() != CodeParser::TokenType::COMMENT) {
                    if (code[idx].getToken() != "(" && code[idx].getToken() != ")" && code[idx].getToken() != "," && code[idx - 1].getToken() != "(" &&
                        code[idx].getToken() != ";") {
                        init.append(" ");
                    }
                    init.append(code[idx].getToken());
                }
                idx++;
            }
            info.insert(std::make_pair(InfoType::FULL_NAME, init));
            info.insert(std::make_pair(InfoType::SHORT_NAME, code[object_idx + 1].getToken()));
        }
    }
    else {
        int stop_idx = static_cast<int>(object_idx) - 1;
        std::vector<CodeParser::Token> cur_object_with_info;
        for (int i = stop_idx - 1; i >= 0; i--) {
            if (code[i].getType() != CodeParser::TokenType::COMMENT && code[i].getToken() != "*new_line*") {
                stop_idx++;
                break;
            }
            else {
                stop_idx = i;
            }
        }
        size_t codeSize = code.size();
        bool insideObjectDefinition = false;
        size_t countBrackets = 0;
        for (size_t i = stop_idx; i < codeSize; i++) {
            CodeParser::Token currentToken = code[i];
            if (!countBrackets && insideObjectDefinition) {
                break;
            }
            else {
                cur_object_with_info.emplace_back(currentToken);
                if (currentToken.getType() == CodeParser::TokenType::OPEN_BRACKET) {
                    countBrackets++;
                    if (!insideObjectDefinition) {
                        insideObjectDefinition = true;
                    }
                }
                else if (currentToken.getType() == CodeParser::TokenType::CLOSE_BRACKET) {
                    countBrackets--;
                }
            }
        }

        size_t definitionStart = 0;
        std::string full_name, short_name;
        bool isShortNameSaved = false;
        for (size_t i = object_idx; i < codeSize; i++) {
            if (code[i].getType() != CodeParser::TokenType::CLASS_DEF && code[i].getType() != CodeParser::TokenType::ENUM_DEF &&
                code[i].getType() != CodeParser::TokenType::UNION_DEF && code[i].getType() != CodeParser::TokenType::STRUCT_DEF &&
                code[i].getType() != CodeParser::TokenType::KEYWORD && !isShortNameSaved) {
                short_name = code[i].getToken();
                isShortNameSaved = true;
            }
            else if (code[i].getToken() == "*new_line*") {
                definitionStart = i + 1;
                break;
            }
            full_name.append(code[i].getToken() + " ");
        }

        size_t curObjectSize = cur_object_with_info.size();

        info.insert(std::make_pair(InfoType::TYPE_OF_OBJECT, code[object_idx].getToken()));
        info.insert(std::make_pair(InfoType::FULL_NAME, full_name));
        info.insert(std::make_pair(InfoType::SHORT_NAME, short_name));

        for (size_t i = 0; i < curObjectSize; i++) {
            if (cur_object_with_info[i].getType() == CodeParser::TokenType::COMMENT) {
                info.insert(std::make_pair(InfoType::BRIEF, commentPreprocessing(cur_object_with_info[i].getToken())));
                break;
            }
            if (cur_object_with_info[i].getToken() == code[object_idx].getToken()) {
                break;
            }
        }
        for (size_t i = definitionStart - object_idx; i < curObjectSize; i++) {
            if (cur_object_with_info[i].getType() == CodeParser::TokenType::COMMENT && i > 0) {
                if (cur_object_with_info[i - 1].getToken() == "*new_line*") {
                    std::string temp;
                    for (size_t j = i + 2; j < curObjectSize; j++) {
                        if (cur_object_with_info[j].getToken() == "*new_line*") {
                            std::string temp_with_comment = commentPreprocessing(cur_object_with_info[i].getToken());
                            temp_with_comment.push_back('\n');
                            temp_with_comment.append(temp);
                            info.insert(std::make_pair(InfoType::MEMBER, temp_with_comment));
                            break;
                        }
                        else {
                            if (cur_object_with_info[i].getType() != CodeParser::TokenType::SEMICOLON) {
                                temp.append(cur_object_with_info[j].getToken());
                            }
                            if (cur_object_with_info[j].getType() != CodeParser::TokenType::OPEN_BRACKET &&
                                cur_object_with_info[j].getType() != CodeParser::TokenType::CLOSE_BRACKET &&
                                cur_object_with_info[j].getType() != CodeParser::TokenType::PASS_SYMBOL) {
                                temp.push_back(' ');
                            }
                        }
                    }
                }
                else {
                    std::string temp, temp_with_comment;
                    size_t startIdx = 0;
                    for (int j = i; j >= 0; j--) {
                        if (cur_object_with_info[j].getToken() == "*new_line*") {
                            startIdx = j + 1;
                            break;
                        }
                    }
                    for (size_t j = startIdx; j < i; j++) {
                        if (cur_object_with_info[i].getType() != CodeParser::TokenType::SEMICOLON) {
                            temp.append(cur_object_with_info[j].getToken());
                        }
                        if (cur_object_with_info[j].getType() != CodeParser::TokenType::OPEN_BRACKET && cur_object_with_info[j].getType() != CodeParser::TokenType::CLOSE_BRACKET &&
                            cur_object_with_info[j].getType() != CodeParser::TokenType::PASS_SYMBOL) {
                            temp.push_back(' ');
                        }
                    }
                    temp_with_comment.append(commentPreprocessing(cur_object_with_info[i].getToken()));
                    temp_with_comment.push_back('\n');
                    temp_with_comment.append(temp);
                    info.insert(std::make_pair(InfoType::MEMBER, temp_with_comment));
                }
            }
        }
    }
}

#pragma endregion ObjectInfo

#pragma region Documentation

DocGen::Documentation::Documentation() {
    documentation = std::multimap<ObjectType, ObjectInfo>();
}

std::multimap<DocGen::ObjectType, DocGen::ObjectInfo> DocGen::Documentation::getDocumentation() {
    return documentation;
}

void DocGen::Documentation::findDocumentationForFile(const std::string &PathToFile, const std::unordered_map<Config::ConfigDatatype, std::string> &config) {
    std::vector<CodeParser::Token> tokenizedCode = CodeParser::Token::tokenizeFile(PathToFile, false, false, true, true, true, true, true);
    size_t tokenizedCodeSize = tokenizedCode.size();
    bool usertypeDefinitionFound = false, insideUsertypeDefinition = false;
    int totalBracketBalance = 0, bracketBalanceBeforeUsertype = 0;

    for (size_t i = 0; i < tokenizedCodeSize; i++) {
        if (tokenizedCode[i].getToken() == "class" || tokenizedCode[i].getToken() == "struct" || tokenizedCode[i].getToken() == "enum" ||
            tokenizedCode[i].getToken() == "enum class" || tokenizedCode[i].getToken() == "union") {
            documentation.insert(std::make_pair(DocGen::ObjectType::USERTYPE, DocGen::ObjectInfo(tokenizedCode, i)));
            usertypeDefinitionFound = true;
            bracketBalanceBeforeUsertype = totalBracketBalance;
            continue;
        }
        if (tokenizedCode[i].getToken() == "{") {
            totalBracketBalance++;
            if (usertypeDefinitionFound) {
                insideUsertypeDefinition = true;
            }
        }
        else if (tokenizedCode[i].getToken() == "}") {
            totalBracketBalance--;
            if (insideUsertypeDefinition && totalBracketBalance == bracketBalanceBeforeUsertype) {
                bracketBalanceBeforeUsertype = 0;
                insideUsertypeDefinition = false;
                usertypeDefinitionFound = false;
            }
        }
        else if (((tokenizedCode[i].getType() == CodeParser::TokenType::VARIABLE_NAME && tokenizedCode[i - 1].getType() == CodeParser::TokenType::TYPENAME &&
                   (tokenizedCode[i + 1].getToken() == "=" || tokenizedCode[i + 1].getToken() == ";" || tokenizedCode[i + 1].getToken() == "{")) ||
                  (tokenizedCode[i].getType() == CodeParser::TokenType::FUNCTION_NAME && tokenizedCode[i - 1].getType() == CodeParser::TokenType::TYPENAME &&
                   tokenizedCode[i + 1].getToken() == "(")) && !insideUsertypeDefinition) {
            documentation.insert(std::make_pair(DocGen::ObjectType::GLOBAL_VARIABLE_OR_FUNCTION, DocGen::ObjectInfo(tokenizedCode, i - 1)));
        }
    }
}

std::string DocGen::Documentation::makeHtmlPath(const std::unordered_map<Config::ConfigDatatype, std::string> &config, const std::string &fileName) {
    std::string path = config.find(Config::ConfigDatatype::DOCUMENTATION_PATH)->second;
    if (path[path.size() - 1] == '/' || path[path.size() - 1] == '\\') {
        path += fileName;
    }
    else {
        if (path.find('\\') != std::string::npos) {
            path += "\\" + fileName;
        }
        else if (path.find('/') != std::string::npos) {
            path += "/" + fileName;
        }
        else {
            path += "/" + fileName;
        }
    }
    return path;
}

void DocGen::generateDocumentation(const std::string &configContent) {
    DocGen::Documentation documentation;
    std::unordered_map<Config::ConfigDatatype, std::string> config = Config::processConfig(configContent);
    std::vector<std::string> paths = Config::getAllEnumeratedPaths(configContent,
                                                                   configContent.find(Config::configDataType2string(Config::ConfigDatatype::FILES_TO_PROCESS_PATHS)));
    for (const auto &path: paths) {
        documentation.findDocumentationForFile(path, config);
    }

    std::string docPath = config[Config::ConfigDatatype::DOCUMENTATION_PATH];

    std::string path = DocGen::Documentation::makeHtmlPath(config, "index.html");

    std::ofstream index(path);

    std::string page = StringTools::readFile("../src/AutoDoc/DocPatterns/page_pattern.html");

    std::vector<std::pair<std::ofstream, std::string>> htmlFiles;

    for (const auto &it: documentation.getDocumentation()) {
        if (it.first == DocGen::ObjectType::USERTYPE) {
            for (const auto &info: it.second.getInfo()) {
                if (info.first == DocGen::InfoType::SHORT_NAME && !info.second.empty()) {
                    page.append("<li>\n\t\t<a href=\"" + info.second + ".html" + "\">");
                    std::string heading;
                    for (const auto &another_info: it.second.getInfo()) {
                        if (another_info.first == DocGen::InfoType::FULL_NAME) {
                            if (another_info.second.empty()) {
                                heading = info.second + "</a>\n\t\t</li>";
                            }
                            else {
                                heading = another_info.second + "</a>\n\t\t</li>\"";
                            }
                        }
                    }
                    page.append(heading + "</a>\n\t\t</li>");
                    std::ofstream newFile(DocGen::Documentation::makeHtmlPath(config, info.second + ".html"));
                    htmlFiles.emplace_back(std::move(newFile), heading);
                }
            }
        }
    }

    std::string globalFuncAndVarsPattern = StringTools::readFile("../src/AutoDoc/DocPatterns/global_f&v.html");

    std::string globalFuncAndVarsHtmlPath = DocGen::Documentation::makeHtmlPath(config, "globalFunctionsAndVariables.html");
    std::ofstream globalFunctionsAndVariables(globalFuncAndVarsHtmlPath);

    page.append(globalFuncAndVarsPattern);
    StringTools::replaceAll(page, "#pageSubmenu", globalFuncAndVarsHtmlPath);
    StringTools::replaceAll(page, "%PROJECT_NAME%", config[Config::ConfigDatatype::PROJECT_NAME]);
    std::string content = StringTools::readFile("../src/AutoDoc/DocPatterns/content_pattern.html");
    page.append(content);
    std::string end = StringTools::readFile("../src/AutoDoc/DocPatterns/end_pattern.html");
    index << page << content << end;
    for (auto &it: htmlFiles) {
        it.first << page << "\t\t\t<br><br>\n\t\t\t<h3>" + it.second + "</h3>\n\t\t\t<pre><code class=\"language-cpp\">";
        for (const auto &info: documentation.getDocumentation()) {
            if (info.first == DocGen::ObjectType::USERTYPE) {
                for (const auto &info_element: info.second.getInfo()) {
                    if (info_element.first == DocGen::InfoType::FULL_NAME && !info_element.second.empty()) {
                        std::string temp_str = info_element.second;
                        StringTools::replaceAll(temp_str, "<", "&lt;");
                        StringTools::replaceAll(temp_str, ">", "&gt;");
                        it.first << temp_str << "</code></pre><br><br><h3>Description</h3>";
                        break;
                    }
                }
                for (const auto &info_element: info.second.getInfo()) {
                    if (info_element.first == DocGen::InfoType::BRIEF && !info_element.second.empty()) {
                        it.first << "<p>" << info_element.second << "</p>";
                    }
                }
            }
        }
        it.first << end;
        it.first.close();
    }
    index.close();
    globalFunctionsAndVariables.close();
}

#pragma endregion Documentation