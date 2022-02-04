/*!
    @author Kozulin Ilya
*/

#include "AutoDoc.h"

docgen::ObjectInfo::ObjectInfo()
{
    type = ObjectType::UNKNOWN;
}

void docgen::ObjectInfo::setType(ObjectType _type)
{
    type = _type;
}

void docgen::ObjectInfo::setInfo(const InfoType &_type, const std::string &_doc)
{
    info.insert(std::make_pair(_type, _doc));
}

std::unordered_map<docgen::InfoType, std::string> docgen::ObjectInfo::getInfo() const
{
    return info;
}

std::string docgen::ObjectInfo::infotype2string(docgen::InfoType _type)
{
    switch (_type)
    {
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

/*!
 * @brief deleting spcial symbols, such as '@' in doxygen style or '*' or "//" in comments
 *
 * @param str
 * @return processed string
 */
std::string docgen::ObjectInfo::commentPreprocessing(std::string str)
{
    std::vector<std::string> symbols_to_delete = {"@", "//", " * ", "/*!", "*/", "/*"};
    for (const auto &it: symbols_to_delete)
        StringTools::replaceAll(str, it, "");
    StringTools::replaceAll(str, "*new_line*", "\n");
    return str;
}

/*!
 * @brief getting information about object (class, structure, method, etc) from commentaries before and inside the object
 *
 * @param code std::vector of tokens, created from text of code
 * @param object_name name of object, information about function is about to find
 */
void docgen::ObjectInfo::findRequiredInfo(const std::vector<CodeParser::Token> &code, const size_t &object_idx)
{
    size_t stop_idx = object_idx - 1;
    std::vector<CodeParser::Token> cur_object_with_info;
    for (size_t i = stop_idx - 1; i >= 0; i--)
    {
        if (code[i].getType() != CodeParser::TokenType::COMMENT)
        {
            stop_idx++;
            break;
        }
        else
            stop_idx = i;
    }

    bool insideObjectDefinition = false;
    size_t countBrackets = 0;
    for (size_t i = stop_idx; i < code.size(); i++)
    {
        CodeParser::Token currentToken = code[i];
        if (!countBrackets && insideObjectDefinition)
            break;
        else
        {
            cur_object_with_info.emplace_back(currentToken);
            if (currentToken.getType() == CodeParser::TokenType::OPEN_BRACKET)
            {
                countBrackets++;
                if (!insideObjectDefinition)
                    insideObjectDefinition = true;
            }
            else if (currentToken.getType() == CodeParser::TokenType::CLOSE_BRACKET)
                countBrackets--;
        }
    }

    info.insert(std::make_pair(InfoType::TYPE_OF_OBJECT, code[object_idx].getToken()));

    size_t definition_start = 0;
    std::string full_name, short_name;
    bool isShortNameSaved = false;
    for (size_t i = object_idx; i < code.size(); i++)
    {
        if (code[i].getType() != CodeParser::TokenType::CLASS_DEF && code[i].getType() != CodeParser::TokenType::ENUM_DEF &&
            code[i].getType() != CodeParser::TokenType::UNION_DEF && code[i].getType() != CodeParser::TokenType::STRUCT_DEF &&
            code[i].getType() != CodeParser::TokenType::KEYWORD && !isShortNameSaved)
        {
            short_name = code[i].getToken();
            isShortNameSaved = true;
        }
        else if (code[i].getToken() == "*new_line*")
        {
            definition_start = i + 1;
            break;
        }
        full_name.append(code[i].getToken() + " ");
    }

    info.insert(std::make_pair(InfoType::TYPE_OF_OBJECT, code[object_idx].getToken()));
    info.insert(std::make_pair(InfoType::FULL_NAME, full_name));
    info.insert(std::make_pair(InfoType::SHORT_NAME, short_name));

    if (cur_object_with_info[0].getType() == CodeParser::TokenType::COMMENT) // -> Doxygen or just simple brief description
        info.insert(std::make_pair(InfoType::BRIEF, commentPreprocessing(cur_object_with_info[0].getToken())));
    for (size_t i = 0; i < cur_object_with_info.size(); i++)
    {
        if (cur_object_with_info[i].getType() == CodeParser::TokenType::COMMENT)
        {
            std::string temp;
            for (size_t j = i + 2; j < cur_object_with_info.size(); j++)
            {
                if (cur_object_with_info[j].getToken() == "*new_line*")
                {
                    std::string temp_with_comment = commentPreprocessing(cur_object_with_info[i].getToken());
                    temp_with_comment.push_back('\n');
                    temp_with_comment.append(temp);
                    info.insert(std::make_pair(InfoType::MEMBER, temp_with_comment));
                    break;
                }
                else
                {
                    temp.append(cur_object_with_info[j].getToken());
                    if (cur_object_with_info[j].getType() != CodeParser::TokenType::OPEN_BRACKET &&
                        cur_object_with_info[j].getType() != CodeParser::TokenType::CLOSE_BRACKET &&
                        cur_object_with_info[j].getType() != CodeParser::TokenType::PASS_SYMBOL)
                    {
                        temp.push_back(' ');
                    }
                }
            }
        }
    }
}

docgen::Documentation::Documentation()
{
    documentation = std::map<ObjectType, ObjectInfo>();
}

/*!
 * @brief makes file structure using "sys/stat.h"
 *
 *  -> *project_name* -> doc -> objects -> class1.html, class2.html, ...
 *                           -> index.html
 *                           -> doc.md
 *                           -> doc.pdf
 */
void docgen::Documentation::createDocFilesStructure()
{
//    fs::create_directories(Constants::objects_path);
//    std::ofstream index(Constants::index_path);
//    std::ifstream index_pattern(Constants::index_start_pattern_path);
//    std::string line;
//    if (index && index_pattern)
//        while (getline(index_pattern, line))
//            index << line << std::endl;
//    index.close();
//    index_pattern.close();
//    std::ofstream menu(Constants::sidebar_path);
//    std::ifstream menu_pattern(Constants::sidebar_start_pattern_path);
//    if (menu && menu_pattern)
//        while (getline(menu_pattern, line))
//            menu << line << std::endl;
//    menu.close();
//    menu_pattern.close();
}

void docgen::Documentation::makeMarkdown(const Documentation &objects)
{

}

void docgen::Documentation::makeHTML(const docgen::Documentation &objects)
{

}


void docgen::Documentation::createDocumentation(const std::string &PathToFile)
{

}
