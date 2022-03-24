#include "Linter.h"
#include "../Utils/ConfigProcessing.h"

int main() {
    auto s = Config::readConfig("config.yml");
    std::cout << s << std::endl;
}

