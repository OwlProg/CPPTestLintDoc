/*!
    \author Kozulin Ilya
    \todo Implement documentation class methods - creating file structure, generating .html, .md and .pdf
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

        case InfoType::NAME:
            return "Name";

        case InfoType::NAMESPACE:
            return "Namespace";

        case InfoType::BRIEF:
            return "Brief";

        case InfoType::AUTHORS:
            return "Authors";

        case InfoType::VERSION:
            return "Version";

        case InfoType::DATE:
            return "Date";

        case InfoType::BUG:
            return "Bus";

        case InfoType::WARNING:
            return "Warning";

        case InfoType::COPYRIGHT:
            return "Copyright";

        case InfoType::EXAMPLE:
            return "Example";

        case InfoType::TODO:
            return "TODO";

        case InfoType::PARAMETER:
            return "Parameter";

        case InfoType::RETURN:
            return "Return";

        case InfoType::MEMBER_FUNCTION:
            return "Member function";

        case InfoType::MEMBER_CONSTANT:
            return "Member constant";
    }
}

/*!
 *\brief getting information about object (class, structure, method, etc) from commentaries before and inside the object
 *\param code std::vector of tokens, created from text of code
 *\param object_name name of object, information about function is about to find
 *\param _type
 */
void docgen::ObjectInfo::findRequiredInfo(const std::vector<CodeParser::Token> &code, const std::string &object_name, const InfoType &_type)
{
    for (int i = 1; i < code.size(); i++)
    {
        if (code[i - 1].type() == infotype2string(_type) && code[i].getToken() == object_name)
        {
            for (int j = i - 1; j >= 0; j--)
            {
                if (code[j].getType() == CodeParser::TokenType::COMMENT)
                {
                    std::vector<CodeParser::Token> current_comment = CodeParser::Token::TokenizeText(code[j].getToken());
                    for (auto &it: current_comment)
                    {
                        if (it.getToken() == "@author" || it.getToken() == "\\author" || it.getToken() == "@authors" || it.getToken() == "\\authors")
                        {
                            // TODO
                        }
                    }
                }
                else if (code[j].getType() != CodeParser::TokenType::COMMENT && code[j].getType() != CodeParser::TokenType::PASS_SYMBOL)
                    break;
            }
        }
    }
}

docgen::Documentation::Documentation()
{
    documentation = std::map<ObjectType, ObjectInfo>();
}

/*!
 * \brief makes file structure using "sys/stat.h"
 *
 *  -> *project_name* -> doc -> objects -> class1.html, class2.html, ...
 *                           -> index.html
 *                           -> doc.md
 *                           -> doc.pdf
 */
void docgen::Documentation::createDocFilesStructure()
{
    // TODO
}

/*!
 * \brief generates index.html for documentation
 */
void docgen::Documentation::generateIndex()
{
    // TODO
}

void docgen::Documentation::generateDocForAnObject(const docgen::ObjectInfo &obj)
{
    // TODO
}

void docgen::Documentation::makeMarkdown()
{
    // TODO
}

void docgen::Documentation::makeFullHtmlDoc()
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
