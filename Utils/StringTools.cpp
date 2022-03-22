/*!
 *  @author Ailurus
 */

#include "StringTools.h"

void StringTools::replaceAll(std::string &data, const std::string &toSearch, const std::string &replaceStr) {
    size_t pos = data.find(toSearch);
    while (pos != std::string::npos) {
        data.replace(pos, toSearch.size(), replaceStr);
        pos = data.find(toSearch, pos + replaceStr.size());
    }
}

std::string StringTools::readFile(const std::string &path) {
    std::ifstream file;
    file.open(path);

    std::stringstream temp;
    temp << file.rdbuf();

    std::string result = temp.str();

    temp.clear();
    file.close();

    return result;
}
