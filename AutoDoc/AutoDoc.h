/*!
 *  @author Ailurus
 */

#ifndef CPPTESTLINTDOC_AUTODOC_H
#define CPPTESTLINTDOC_AUTODOC_H

#include <unordered_map>
#include <map>
#include <regex>
#include <string_view>

#include "../Utils/Tokenizer.h"
#include "../Utils/StringTools.h"
#include "../Utils/ConfigProcessing.h"

namespace DocGen
{

    enum class ObjectType
    {
        USERTYPE,
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

    std::string objecttype2string(const ObjectType &objectType);

    std::string infotype2string(const InfoType &infoType);

    class ObjectInfo
    {
    protected:

        ObjectType type;

        std::multimap<InfoType, std::string> info;

    public:

        ObjectInfo();

        void setType(const ObjectType &_type);

        void setInfo(const InfoType &_type, const std::string &_doc);

        [[nodiscard]] std::multimap<InfoType, std::string> getInfo() const;

        /*!
         * @brief deleting spcial symbols, such as '@' in doxygen style or '*' or "//" in comments
         *
         * @param str
         * @return processed string
         */
        static std::string commentPreprocessing(std::string str);

        /*!
         * @brief constructor for getting information about object (class, structure, method, etc) from commentaries before and inside the object
         *
         * @param code std::vector of tokens, created from text of code
         * @param object_name name of object, information about function is about to find
         */
        ObjectInfo(const std::vector<CodeParser::Token> &code, const size_t &object_idx);
    };

    class Documentation
    {
    private:

        std::multimap<ObjectType, ObjectInfo> documentation;

    public:

        Documentation();

        std::multimap<ObjectType, ObjectInfo> getDocumentation();

        /*!
         * @brief the main function in document generation part
         *
         * @param PathToFile
         */
        void findDocumentationForFile(const std::string &PathToFile, const std::unordered_map<Config::ConfigDatatype, std::string> &config);

        static std::string makeHtmlPath(const std::unordered_map<Config::ConfigDatatype, std::string> &config, const std::string &fileName);
    };

    void generateDocumentation(const std::string &configContent);
}

namespace Constants
{
    inline constexpr std::string_view page_pattern_path = "../src/AutoDoc/DocPatterns/page_pattern.html";

    inline constexpr std::string_view end_pattern_path = "../src/AutoDoc/DocPatterns/end_pattern.html";

    inline constexpr std::string_view globals_path = "../src/AutoDoc/DocPatterns/golbal_f&v.html";

    inline constexpr std::string_view content_pattern_path = "../src/AutoDoc/DocPatterns/content_pattern.html";
}


#endif //CPPTESTLINTDOC_AUTODOC_H
