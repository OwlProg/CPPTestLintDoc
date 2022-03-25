/*!
 *  @author Ailurus
 */


#include "ConfigProcessing.h"

std::string Config::configDataType2string(const Config::ConfigDatatype &configDatatype) {
    switch (configDatatype) {
        case ConfigDatatype::PROJECT_NAME:
            return "ProjectName";

        case ConfigDatatype::ROOT_PATH:
            return "RootPath";

        case ConfigDatatype::LOGO_PATH:
            return "LogoPath";

        case ConfigDatatype::DOCUMENTATION_PATH:
            return "DocumentationPath";

        case ConfigDatatype::FILES_TO_PROCESS_PATHS:
            return "FilesToProcessPaths";

        case ConfigDatatype::REPOSITORY_URL:
            return "RepositoryURL";

        case ConfigDatatype::FILES_FOR_LINTER:
            return "FilesToLinterCheck";

        default:
            return "Unknown";
    }
}

std::string Config::readConfig(const std::string &configPath) {
    std::unordered_map<Config::ConfigDatatype, std::string> config;
    std::ifstream file;
    file.open(configPath, std::ifstream::in);

    std::stringstream temp;
    temp << file.rdbuf();

    file.close();

    return temp.str();
}

std::vector<std::string> Config::getAllEnumeratedPaths(const std::string &str, const size_t &idx) {
    size_t stringSize = str.size();
    int endOfPathEnumeration = -1;
    std::vector<std::string> paths;
    bool insideQuotes = false, enumViaBrackets = false;
    for (size_t i = idx; i < stringSize; i++) {
        if (str[i] == '[') {
            enumViaBrackets = true;
            break;
        }
        else if (str[i] == '-') {
            break;
        }
    }
    if (enumViaBrackets) {
        for (size_t i = idx; i < stringSize; i++) {
            if (str[i] == ']') {
                endOfPathEnumeration = i;
                break;
            }
        }
    }
    else {
        int firstLineOfEnumeration = -1;
        for (int i = idx; i < stringSize; i++) {
            if (str[i] == '\n') {
                firstLineOfEnumeration = i;
                break;
            }
        }
        bool endFound = false, atLeastOneDashFound = false;
        for (int i = firstLineOfEnumeration; i < stringSize; i++) {
            if (str[i] == '\n') {
                bool dashFound = false;
                for (int j = i + 1; j < stringSize; j++) {
                    if (str[j] == '-') {
                        atLeastOneDashFound = true;
                        dashFound = true;
                    }
                    if (str[j] == '\n') {
                        if (!dashFound) {
                            endOfPathEnumeration = i;
                            endFound = true;
                            break;
                        }
                        else {
                            i = j - 1;
                            break;
                        }
                    }
                }
            }
            if (endFound) {
                break;
            }
        }
        if (atLeastOneDashFound && endOfPathEnumeration == -1) {
            endOfPathEnumeration = stringSize;
        }
    }
    std::string curPath;
    if (endOfPathEnumeration == -1) {
        throw Exceptions::ExceptionWithDescription(std::string(Constants::config_paths_processing_error));
    }
    else {
        for (size_t i = idx; i < endOfPathEnumeration; i++) {
            if (str[i] == '"') {
                if (!insideQuotes) {
                    insideQuotes = true;
                }
                else {
                    insideQuotes = false;
                    paths.emplace_back(curPath);
                    curPath.clear();
                }
            }
            else {
                if (insideQuotes) {
                    curPath.push_back(str[i]);
                }
                else {
                    continue;
                }
            }
        }
        if (!insideQuotes) {
            return paths;
        }
        else {
            throw Exceptions::ExceptionWithDescription(std::string(Constants::config_quotes_error));
        }
    }
}

std::pair<std::string, size_t> Config::findNextWord(const std::string &str, const size_t &idx) {
    size_t stringSize = str.size();
    std::pair<std::string, size_t> nextWord;
    bool firstWordEnded = false, secondWordStarted = false;
    for (size_t i = idx; i < stringSize; i++) {
        if (str[i] == ':' && !firstWordEnded) {
            firstWordEnded = true;
        }
        else if (str[i] == '"' && firstWordEnded) {
            for (size_t j = i + 1; j < stringSize; j++) {
                if (str[j] != '"') {
                    nextWord.first.push_back(str[j]);
                }
                else {
                    nextWord.second = i - 1;
                    return nextWord;
                }
            }
        }
        else if ((str[i] >= 'a' && str[i] <= 'z' || str[i] >= 'A' && str[i] <= 'Z') && firstWordEnded) {
            if (!secondWordStarted) {
                nextWord.second = i;
                secondWordStarted = true;
            }
            nextWord.first.push_back(str[i]);
        }
        else {
            if (secondWordStarted) {
                return nextWord;
            }
            else {
                continue;
            }
        }
    }
    return nextWord;
}

std::unordered_map<Config::ConfigDatatype, std::string> Config::processConfig(const std::string &configContent) {
    std::unordered_map<Config::ConfigDatatype, std::string> config;
    std::string allConfigDatatypes;
    for (const Config::ConfigDatatype &datatype: Config::configDatatypes) {
        allConfigDatatypes.append(configDataType2string(datatype) + ", ");
    }

    for (const Config::ConfigDatatype &datatype: Config::configDatatypes) {
        size_t idx = configContent.find(configDataType2string(datatype));

        if (idx != std::string::npos) {
            if (datatype != Config::ConfigDatatype::FILES_TO_PROCESS_PATHS
            && datatype != Config::ConfigDatatype::FILES_FOR_LINTER) {
                std::string str = (Config::findNextWord(configContent, idx)).first;
                config[datatype] = str;
                if (config[datatype][0] == '"' && config[datatype][config[datatype].size() - 1] == '"') {
                    config[datatype].pop_back(), config[datatype].erase(0, 1);
                }
            }
            else {
                std::vector<std::string> paths = getAllEnumeratedPaths(configContent, idx);
                for (const auto &it: paths) {
                    config[datatype].append(it + ' ');
                }
            }
        }
        else {
            throw Exceptions::ExceptionWithDescription(std::string(Constants::config_arguement_not_found_error_start) + configDataType2string(datatype) +
                                                       std::string(Constants::config_arguement_not_found_error_end) + allConfigDatatypes);
        }
    }
    return config;
}