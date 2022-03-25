/*!
 *  @author Ailurus
 */

#include "AutoDoc.h"

int main() {
    std::string configContent = Config::readConfig(std::string(Constants::config_path));
    DocGen::generateDocumentation(configContent);
    std::cout << "Documentation has been successfully generated";
    return 0;
}