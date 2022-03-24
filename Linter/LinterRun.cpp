#include "FileLinter.h"
#include "../Utils/ConfigProcessing.h"
#include <unordered_set>

constexpr std::ostream& outputStream = std::cout; // output

int main() {
    auto conf = Config::readConfig("config.yml");
    auto preparedConf = Config::processConfig(conf);
    std::string files = preparedConf[Config::ConfigDatatype::FILES_FOR_LINTER];
    std::string delimiter = " ";
    size_t pos = 0;
    std::string token;
    while ((pos = files.find(delimiter)) != std::string::npos) {
        token = files.substr(0, pos);
        Linter::FileLinter file(token);
        outputStream << "Analysing " << token << "..." << std::endl;
        file.detailedReport(outputStream);
        files.erase(0, pos + delimiter.length());
    }
}

