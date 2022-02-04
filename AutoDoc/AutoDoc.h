/*!
    @author Kozulin Ilya
*/

#ifndef CPPTESTLINTDOC_AUTODOC_H
#define CPPTESTLINTDOC_AUTODOC_H

//#include <experimental/filesystem>
#include <unordered_map>
#include <map>
#include <regex>
#include <string_view>

#include "../Utils/Tokenizer.h"
#include "../Utils/StringTools.h"

//namespace fs = std::filesystem;

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

        ObjectInfo();

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

        static void createDocFilesStructure();

        void makeMarkdown(const Documentation &objects);

        void makeHTML(const Documentation &objects);

        void createDocumentation(const std::string &PathToFile);
    };
}

namespace Constants
{
    using namespace std::literals;

    inline constexpr std::string_view objects_path = "docs/objects";

    inline constexpr std::string_view index_path = "docs/index.html";

    inline constexpr std::string_view sidebar_path = "docs/objects/menu.html";

    inline constexpr std::string_view index_start_pattern_path = "AutoDoc/DocPatterns/index_start_pattern.html";

    inline constexpr std::string_view index_end_pattern_path = "AutoDoc/DocPatterns/index_end_pattern.html";

    inline constexpr std::string_view sidebar_start_pattern_path = "AutoDoc/DocPatterns/menu_start_pattern.html";

    inline constexpr std::string_view sidebar_end_pattern_path = "AutoDoc/DocPatterns/menu_end_pattern.html";
}


#endif //CPPTESTLINTDOC_AUTODOC_H
