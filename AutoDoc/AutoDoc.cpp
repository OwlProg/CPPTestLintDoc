/*!
 *  @author Ailurus
 */

#include "AutoDoc.h"

#pragma region ObjectInfo

DocGen::ObjectInfo::ObjectInfo() {
    info = std::multimap<InfoType, std::string>();
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
                    if (code[idx].getToken() != "{") {
                        init.append(code[idx].getToken());
                    }
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
        if (std::count(full_name.begin(), full_name.end(), '{')) {
            int balance = 1;
            full_name.append("\n\t");
            for (int j = definitionStart; j < codeSize; j++) {
                if (code[j].getToken() == "{") {
                    balance++;
                }
                else if (code[j].getToken() == "}") {
                    balance--;
                }
                if (code[j].getToken() == "}" && balance == 0) {
                    break;
                }
                else if (code[j].getToken() == "*new_line*" || code[j].getToken() == "\n") {
                    std::string temp = "\n";
                    for (int t = 0; t < balance; t++) {
                        temp.append("\t");
                    }
                    full_name.append(temp);
                }
                else if (code[j].getToken() == "." || code[j].getToken() == "," || code[j].getToken() == ";" || code[j].getToken() == "(" || code[j].getToken() == ")" ||
                         code[j].getToken() == "[" || code[j].getToken() == "]" || code[j].getToken() == ":") {
                    if (full_name[full_name.size() - 1] == ' ') {
                        full_name.pop_back();
                    }
                    full_name.append(code[j].getToken());
                    if (code[j].getToken() == "," || code[j].getToken() == ")") {
                        full_name.append(" ");
                    }
                }
                else {
                    full_name.append(code[j].getToken() + " ");
                }
            }
        }
        for (size_t i = 1; i < full_name.size(); i++) {
            if (full_name[i] == '}' && full_name[i - 1] == '\t') {
                full_name[i - 1] = '}';
                full_name[i] = '\t';
            }
        }
        while (full_name.find("\tpublic:") != std::string::npos) {
            StringTools::replaceAll(full_name, "\tpublic:", "public:");
        }
        while (full_name.find("\tprivate:") != std::string::npos) {
            StringTools::replaceAll(full_name, "\t\tprivate:", "private:");
        }
        while (full_name.find("\tprotected:") != std::string::npos) {
            StringTools::replaceAll(full_name, "\tprotected:", "protected:");
        }
        full_name.append("};");
        while (full_name.find("\t};") != std::string::npos) {
            StringTools::replaceAll(full_name, "\t};", "};");
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
                        if (another_info.first == DocGen::InfoType::TYPE_OF_OBJECT) {
                            heading = another_info.second;
                        }
                    }
                    for (const auto &another_info: it.second.getInfo()) {
                        if (another_info.first == DocGen::InfoType::SHORT_NAME) {
                            if (another_info.second.empty()) {
                                heading += " " + info.second;
                            }
                            else {
                                heading += " " + another_info.second;
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

    std::string globalFuncAndVarsPattern = StringTools::readFile(std::string(Constants::globals_path));
    std::string content = StringTools::readFile(std::string(Constants::content_pattern_path));
    std::string end = StringTools::readFile(std::string(Constants::end_pattern_path));
    std::string globalFuncAndVarsHtmlPath = DocGen::Documentation::makeHtmlPath(config, "globalFunctionsAndVariables.html");

    std::ofstream globalFunctionsAndVariables(globalFuncAndVarsHtmlPath);

    page.append(globalFuncAndVarsPattern);
    page.append(content);

    StringTools::replaceAll(page, "%GLOBALS_PAGE%", globalFuncAndVarsHtmlPath);
    StringTools::replaceAll(page, "%PROJECT_NAME%", config[Config::ConfigDatatype::PROJECT_NAME]);
    StringTools::replaceAll(page, "%LOGO_PATH%", config[Config::ConfigDatatype::LOGO_PATH]);
    StringTools::replaceAll(page, "%REPOSITORY_URL%", config[Config::ConfigDatatype::REPOSITORY_URL]);

    index << page << content;

    for (auto &it: htmlFiles) {
        it.first << page << "\t\t\t<br><br>\n\t\t\t<div id=\"heading\"><h3>" + it.second + "</h3></div>\n\t\t\t<pre><code class=\"language-cpp\">";
        for (const auto &info: documentation.getDocumentation()) {
            if (info.first == DocGen::ObjectType::USERTYPE) {
                auto docSource = info.second.getInfo();
                if (docSource.find(DocGen::InfoType::TYPE_OF_OBJECT)->second + " " + docSource.find(DocGen::InfoType::SHORT_NAME)->second == it.second) {
                    for (const auto &info_element: info.second.getInfo()) {
                        if (info_element.first == DocGen::InfoType::FULL_NAME && !info_element.second.empty()) {
                            std::string temp_str = info_element.second;
                            StringTools::replaceAll(temp_str, "<", "&lt;");
                            StringTools::replaceAll(temp_str, ">", "&gt;");
                            it.first << temp_str << "</code></pre><br><br><div id=\"heading\"><h3>Description</h3></div>";
                            break;
                        }
                    }
                    for (const auto &info_element: info.second.getInfo()) {
                        if (info_element.first == DocGen::InfoType::BRIEF && !info_element.second.empty()) {
                            it.first << "<p>" << info_element.second << "</p>";
                            break;
                        }
                    }
                }
                else {
                    continue;
                }
            }
        }
        it.first << end;
        it.first.close();
    }

    globalFunctionsAndVariables << page << "\t\t\t<br><br>\n\t\t\t<div id=\"heading\"><h3>Global Functions and Variables</h3></div>" << content;

    int countMembers = 1;
    for (const auto &info: documentation.getDocumentation()) {
        if (info.first == DocGen::ObjectType::GLOBAL_VARIABLE_OR_FUNCTION) {
            auto docSource = info.second.getInfo();
            if (!docSource.find(DocGen::InfoType::FULL_NAME)->second.empty()) {
                if (countMembers == 1) {
                    globalFunctionsAndVariables << "<div id=\"heading\"><h3 style=\"margin-top: -150px\">" + std::to_string(countMembers) + ") " +
                                                   docSource.find(DocGen::InfoType::TYPE_OF_OBJECT)->second + " " + docSource.find(DocGen::InfoType::SHORT_NAME)->second +
                                                   "</h3></div>\n\t\t\t<pre><code class=\"language-cpp\">";
                }
                else {
                    globalFunctionsAndVariables
                            << "<div id=\"heading\"><h3>" + std::to_string(countMembers) + ") " + docSource.find(DocGen::InfoType::TYPE_OF_OBJECT)->second + " " +
                               docSource.find(DocGen::InfoType::SHORT_NAME)->second + "</h3></div>\n\t\t\t<pre><code class=\"language-cpp\">";
                }
                countMembers++;
                for (const auto &info_element: info.second.getInfo()) {
                    if (info_element.first == DocGen::InfoType::FULL_NAME && !info_element.second.empty()) {
                        std::string temp_str = info_element.second;
                        StringTools::replaceAll(temp_str, "<", "&lt;");
                        StringTools::replaceAll(temp_str, ">", "&gt;");
                        globalFunctionsAndVariables << temp_str << "</code></pre><div id=\"heading\"><h3>Description</h3></div>";
                        break;
                    }
                }
                for (const auto &info_element: info.second.getInfo()) {
                    if (info_element.first == DocGen::InfoType::BRIEF && !info_element.second.empty()) {
                        globalFunctionsAndVariables << "<p>" << info_element.second << "</p><br><br>";
                        break;
                    }
                }
            }
        }
    }

    globalFunctionsAndVariables << end;

    index << "\t\t\t<br><br>\n\t\t\t<div id=\"heading\"><h3 style=\"margin-top: -225px\">" << config[Config::ConfigDatatype::PROJECT_NAME] << "</h3></div>"
          << "<p>Welcome to the project " << config[Config::ConfigDatatype::PROJECT_NAME] << "!</p>\n<p>You can find the source code <a style=\"color: blue\"href="
          << config[Config::ConfigDatatype::REPOSITORY_URL] << ">here</a></p>\n"
          << "<p style=\"margin-top: 425px\">This documentation was automatically genereted by CPPTestLintDoc tool. Follow the <a style=\"color: blue\"href=https://github.com/OwlProg/CPPTestLintDoc>link</a> to find more.</p>\n"
          << end;

    index.close();
    globalFunctionsAndVariables.close();

}

#pragma endregion Documentation