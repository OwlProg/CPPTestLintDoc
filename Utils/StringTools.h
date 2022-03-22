/*!
 *  @author Ailurus
 */


#ifndef CPPTESTLINTDOC_STRINGTOOLS_H
#define CPPTESTLINTDOC_STRINGTOOLS_H

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <string_view>
#include <fstream>
#include <sstream>
#include <ostream>

namespace StringTools {
    /*!
     * @brief replace all occurrences of a substring in a string
     *
     * @param data a string where you want to make replacing
     * @param toSearch a string you want to replace
     * @param replaceStr a string you want to have in the place of toSearch
     */
    void replaceAll(std::string &data, const std::string &toSearch, const std::string &replaceStr);

    /*!
     * @brief converts text in file into std::string
     *
     * @param path
     * @return string with text from the file
     */
    std::string readFile(const std::string &path);
}


#endif //CPPTESTLINTDOC_STRINGTOOLS_H
