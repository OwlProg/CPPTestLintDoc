/*!
 *  @author Ailurus
 */

#include "AutoDoc.h"

int main() {
    std::string configContent = Config::readConfig(std::string(Constants::config_path));
    std::unordered_map<Config::ConfigDatatype, std::string> config = Config::processConfig(configContent);
    DocGen::Documentation documentation;
    std::vector<std::string> paths = Config::getAllEnumeratedPaths(configContent,
                                                                   configContent.find(Config::configDataType2string(Config::ConfigDatatype::FILES_TO_PROCESS_PATHS)));
    for (const auto &path: paths) {
        documentation.createDocumentation(path, config);
    }
    return 0;
}