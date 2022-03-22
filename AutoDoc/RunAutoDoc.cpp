/*!
 *  @author Ailurus
 */

#include "AutoDoc.h"

int main() {
    std::string configContent = Config::readConfig(std::string(Constants::config_path));
    DocGen::generateDocumentation(configContent);
    return 0;
}