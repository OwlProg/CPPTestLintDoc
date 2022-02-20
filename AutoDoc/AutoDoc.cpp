/*!
 *  @author Ailurus
 */

#include "AutoDoc.h"

DocGen::ObjectInfo::ObjectInfo() {
    type = ObjectType::UNKNOWN;
}

void DocGen::ObjectInfo::setType(const ObjectType &_type) {
    type = _type;
}

void DocGen::ObjectInfo::setInfo(const InfoType &_type, const std::string &_doc) {
    info.insert(std::make_pair(_type, _doc));
}

std::unordered_map<DocGen::InfoType, std::string> DocGen::ObjectInfo::getInfo() const {
    return info;
}

std::string DocGen::ObjectInfo::infotype2string(const DocGen::InfoType &_type) {
    switch (_type) {
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

void DocGen::ObjectInfo::findRequiredInfo(const std::vector<CodeParser::Token> &code, const size_t &object_idx) {
    size_t stop_idx = object_idx - 1;
    std::vector<CodeParser::Token> cur_object_with_info;
    for (size_t i = stop_idx - 1; i >= 0; i--) {
        if (code[i].getType() != CodeParser::TokenType::COMMENT) {
            stop_idx++;
            break;
        }
        else {
            stop_idx = i;
        }
    }

    bool insideObjectDefinition = false;
    size_t countBrackets = 0;
    for (size_t i = stop_idx; i < code.size(); i++) {
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

    info.insert(std::make_pair(InfoType::TYPE_OF_OBJECT, code[object_idx].getToken()));

    size_t definition_start = 0;
    std::string full_name, short_name;
    bool isShortNameSaved = false;
    for (size_t i = object_idx; i < code.size(); i++) {
        if (code[i].getType() != CodeParser::TokenType::CLASS_DEF && code[i].getType() != CodeParser::TokenType::ENUM_DEF &&
            code[i].getType() != CodeParser::TokenType::UNION_DEF && code[i].getType() != CodeParser::TokenType::STRUCT_DEF &&
            code[i].getType() != CodeParser::TokenType::KEYWORD && !isShortNameSaved) {
            short_name = code[i].getToken();
            isShortNameSaved = true;
        }
        else if (code[i].getToken() == "*new_line*") {
            definition_start = i + 1;
            break;
        }
        full_name.append(code[i].getToken() + " ");
    }

    info.insert(std::make_pair(InfoType::TYPE_OF_OBJECT, code[object_idx].getToken()));
    info.insert(std::make_pair(InfoType::FULL_NAME, full_name));
    info.insert(std::make_pair(InfoType::SHORT_NAME, short_name));

    if (cur_object_with_info[0].getType() == CodeParser::TokenType::COMMENT) { // -> Doxygen or just simple brief description
        info.insert(std::make_pair(InfoType::BRIEF, commentPreprocessing(cur_object_with_info[0].getToken())));
    }
    for (size_t i = 0; i < cur_object_with_info.size(); i++) {
        if (cur_object_with_info[i].getType() == CodeParser::TokenType::COMMENT) {
            std::string temp;
            for (size_t j = i + 2; j < cur_object_with_info.size(); j++) {
                if (cur_object_with_info[j].getToken() == "*new_line*") {
                    std::string temp_with_comment = commentPreprocessing(cur_object_with_info[i].getToken());
                    temp_with_comment.push_back('\n');
                    temp_with_comment.append(temp);
                    info.insert(std::make_pair(InfoType::MEMBER, temp_with_comment));
                    break;
                }
                else {
                    temp.append(cur_object_with_info[j].getToken());
                    if (cur_object_with_info[j].getType() != CodeParser::TokenType::OPEN_BRACKET &&
                        cur_object_with_info[j].getType() != CodeParser::TokenType::CLOSE_BRACKET &&
                        cur_object_with_info[j].getType() != CodeParser::TokenType::PASS_SYMBOL) {
                        temp.push_back(' ');
                    }
                }
            }
        }
    }
}

DocGen::Documentation::Documentation() {
    documentation = std::map<ObjectType, ObjectInfo>();
}

void DocGen::Documentation::makeMarkdown(const Documentation &objects) {

}

void DocGen::Documentation::makeHTML(const DocGen::Documentation &objects) {

}


void DocGen::Documentation::createDocumentation(const std::string &PathToFile, const std::unordered_map<Config::ConfigDatatype, std::string> &config) {
    std::vector<CodeParser::Token> tokenizedCode = CodeParser::Token::tokenizeFile(PathToFile, false, false, true, true, true, true, true);

}
