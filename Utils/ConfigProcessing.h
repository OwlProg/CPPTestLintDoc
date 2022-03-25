/*!
 *  @author Ailurus
 */

#ifndef CPPTESTLINTDOC_CONFIGPROCESSING_H
#define CPPTESTLINTDOC_CONFIGPROCESSING_H

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Exceptions.h"

namespace Constants {

    constexpr size_t numberOfSpecialYamlSymbols = 8, numberOfConfigDatatypes = 7;

    const std::array<char, numberOfSpecialYamlSymbols> specialYamlSymbols = {',', ' ', '\n', '\t', ':', '[', '-'};

    inline constexpr std::string_view config_path = "../CPPTestLintDoc/config.yml";

    inline constexpr std::string_view config_arguement_not_found_error_start = "An error was detected during the processing of the config.yaml: ";

    inline constexpr std::string_view config_arguement_not_found_error_end = " not found.\nPlease check that the following items are present in the configuration file: ";

    inline constexpr std::string_view config_quotes_error = "The number of quotation marks is odd. Please, check the correctness of entered data (paths must be enclosed in quotation marks)";

    inline constexpr std::string_view config_paths_processing_error = "Something went wrong. Please check the structure of config. Remember, all paths must be inside quote marks and enumerated in [] or with '-' on a few lines";
}

namespace Config {
    enum class ConfigDatatype {
        PROJECT_NAME,
        ROOT_PATH,
        LOGO_PATH,
        DOCUMENTATION_PATH,
        FILES_TO_PROCESS_PATHS,
        REPOSITORY_URL,
        FILES_FOR_LINTER,
        UNKNOWN_DATA
    };

    constexpr std::array<ConfigDatatype, Constants::numberOfConfigDatatypes> configDatatypes = {ConfigDatatype::PROJECT_NAME, ConfigDatatype::ROOT_PATH, ConfigDatatype::LOGO_PATH,
                                                                                                ConfigDatatype::FILES_TO_PROCESS_PATHS, ConfigDatatype::DOCUMENTATION_PATH,
                                                                                                ConfigDatatype::REPOSITORY_URL, ConfigDatatype::FILES_FOR_LINTER};

    std::string configDataType2string(const ConfigDatatype &configDatatype);

    /*!
     * @brief just reads the config.yml file
     *
     * @param configPath
     * @return string of config content
     */
    std::string readConfig(const std::string &configPath);

    /*!
     * @brief separate processing of paths
     *
     * @param str a string where to find paths
     * @param idx an index of a key
     * @return vector of paths
     */
    std::vector<std::string> getAllEnumeratedPaths(const std::string &str, const size_t &idx);

    /*!
     * @brief function mostly for config processing - helps to find a value for a key in yaml
     *
     * @param str a string where to find word
     * @param idx an index of word the previous to that you want to get
     * @return the pair of the next value and its start index
     */
    std::pair<std::string, size_t> findNextWord(const std::string &str, const size_t &idx);

    /*!
     * @brief gets all the values for all known keys of config file
     *
     * @return hashtable with pairs of config data types and their arguments
     */
    std::unordered_map<ConfigDatatype, std::string> processConfig(const std::string &configContent);
}

#endif //CPPTESTLINTDOC_CONFIGPROCESSING_H
