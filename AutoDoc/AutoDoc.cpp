/*!
    @author Kozulin Ilya
*/

#include "AutoDoc.h"

void docgen::ObjectInfo::setType(ObjectType _type)
{
    type = _type;
}

void docgen::ObjectInfo::setInfo(const InfoType &_type, const std::string &_doc)
{
    info[_type] = _doc;
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

        case InfoType::UNKNOWN:
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
    for (auto &it: symbols_to_delete)
        str.erase(std::remove(str.begin(), str.end(), it), str.end());
    std::regex_replace(str, std::regex("*new_line*"), "\n");
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
    for (int i = stop_idx; i >= 0; i--)
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
        if (!countBrackets && insideObjectDefinition)
            break;
        else
        {
            CodeParser::Token currentToken = code[i];
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

    info[InfoType::TYPE_OF_OBJECT] = code[object_idx].getToken();

    size_t definition_start = 0;
    std::string full_name, short_name;
    bool isShortNameSaved = false;
    for (size_t i = object_idx; i < code.size(); i++)
    {
        if (code[i].getType() != CodeParser::TokenType::KEYWORD && !isShortNameSaved)
        {
            short_name = code[i].getToken();
            isShortNameSaved = true;
        }
        else if (code[i].getToken() == "*new_line*")
        {
            definition_start = i + 1;
            break;
        }
        else
            full_name.append(code[i].getToken() + " ");
    }

    info[InfoType::TYPE_OF_OBJECT] = code[object_idx].getToken();
    info[InfoType::FULL_NAME] = full_name;
    info[InfoType::SHORT_NAME] = short_name;

    if (cur_object_with_info[0].getType() == CodeParser::TokenType::COMMENT) // -> Doxygen or just simple brief description
        info[InfoType::BRIEF] = commentPreprocessing(cur_object_with_info[0].getToken());
    else
    {
        if (code[definition_start].getType() == CodeParser::TokenType::COMMENT)
            info[InfoType::BRIEF] = commentPreprocessing(code[definition_start].getToken());
        else
            info[InfoType::BRIEF] = "";
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
    fs::create_directories(Paths::objects_path);
    std::ofstream index(Paths::index_path);
    std::ifstream index_pattern(Paths::index_start_pattern_path);
    std::string line;
    if (index && index_pattern)
        while (getline(index_pattern, line))
            index << line << std::endl;
    index.close();
    index_pattern.close();
    std::ofstream menu(Paths::sidebar_path);
    std::ifstream menu_pattern(Paths::sidebar_start_pattern_path);
    if (menu && menu_pattern)
        while (getline(menu_pattern, line))
            menu << line << std::endl;
    menu.close();
    menu_pattern.close();
}

void docgen::Documentation::generateDocForAnObject(const docgen::ObjectInfo &obj)
{
    // TODO
}

void docgen::Documentation::makeMarkdown()
{
    // TODO
}

void docgen::Documentation::generateDocumentation()
{
    // TODO
}

std::vector<docgen::ObjectInfo> docgen::Documentation::adviseToDocument()
{
    // TODO
    return std::vector<ObjectInfo>();
}

void docgen::Documentation::createDocumentation(const std::string &FileName)
{
    // TODO
}
