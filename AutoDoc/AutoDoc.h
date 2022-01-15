/*!
    \author Kozulin Ilya
    \warning tests needed; generating .html, .md and .pdf still not implemented
*/

#ifndef CPPTESTLINTDOC_AUTODOC_H
#define CPPTESTLINTDOC_AUTODOC_H

#include <filesystem>
#include <unordered_map>
#include <map>

#include "../Utils/Tokenizer.h"

namespace docgen
{

    enum class ObjectType
    {
        CLASS,
        STRUCTURE,
        FUNCTION,
        ENUMERATION,
        UNION,
        NAMESPACE,
        UNKNOWN
    };

    enum class InfoType
    {
        TYPE_OF_OBJECT,
        NAME,
        NAMESPACE,
        BRIEF,
        AUTHORS,
        VERSION,
        DATE,
        BUG,
        WARNING,
        COPYRIGHT,
        EXAMPLE,
        TODO,
        PARAMETER,
        RETURN,
        MEMBER_FUNCTION,
        MEMBER_CONSTANT
    };

    class ObjectInfo
    {
    protected:

        ObjectType type;

        std::unordered_map<InfoType, std::string> info;

    public:

        void setType(ObjectType _type);

        void setInfo(const InfoType &_type, const std::string &_doc);

        std::unordered_map<InfoType, std::string> getInfo() const;

        static std::string infotype2string(InfoType _type);

        static void findRequiredInfo(const std::vector<CodeParser::Token>& code, const std::string& object_name, const InfoType &_type);
    };

    class Documentation
    {
    private:

        std::map<ObjectType, ObjectInfo> documentation;

    public:

        Documentation();

        void createDocFilesStructure();

        void generateIndex();

        void generateDocForAnObject(const ObjectInfo &obj);

        void makeMarkdown();

        void makeFullHtmlDoc();

        void generateDocumentation();

        std::vector<ObjectInfo> adviseToDocument();

        void createDocumentation(const std::string& FileName);
    };
}

#endif //CPPTESTLINTDOC_AUTODOC_H
