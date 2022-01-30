/*!
    @author Kozulin Ilya
*/

#ifndef CPPTESTLINTDOC_AUTODOC_H
#define CPPTESTLINTDOC_AUTODOC_H

#include <filesystem>
#include <unordered_map>
#include <map>
#include <regex>

#include "../Utils/Tokenizer.h"

namespace fs = std::filesystem;

namespace docgen
{

    enum class ObjectType
    {
        CLASS_OR_USERTYPE,
        GLOBAL_VARIABLE_OR_FUNCTION,
        UNKNOWN
    };

    enum class InfoType
    {
        TYPE_OF_OBJECT,
        SHORT_NAME,
        FULL_NAME,
        FILE,
        NAMESPACE,
        BRIEF,
        MEMBER,
        UNKNOWN
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

        static std::string commentPreprocessing(std::string str);

        void findRequiredInfo(const std::vector<CodeParser::Token> &code, const size_t &object_idx);
    };

    class Documentation
    {
    private:

        std::map<ObjectType, ObjectInfo> documentation;

    public:

        Documentation();

        void createDocFilesStructure();

        void generateDocForAnObject(const ObjectInfo &obj);

        void makeMarkdown();

        void generateDocumentation();

        std::vector<ObjectInfo> adviseToDocument();

        void createDocumentation(const std::string &FileName);
    };
}

namespace Constants
{
    class Paths
    {
    public:
        static constexpr char *objects_path = "docs/objects";
        static constexpr char *index_path = "docs/index.html";
        static constexpr char *sidebar_path = "docs/objects/menu.html";
        static constexpr char *index_start_pattern_path = "Utils/index_start_pattern.html";
        static constexpr char *index_end_pattern_path = "Utils/index_end_pattern.html";
        static constexpr char *sidebar_start_pattern_path = "Utils/menu_start_pattern.html";
        static constexpr char *sidebar_end_pattern_path = "Utils/menu_end_pattern.html";
    };
}


#endif //CPPTESTLINTDOC_AUTODOC_H
