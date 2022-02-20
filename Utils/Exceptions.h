/*!
 *  @author Ailurus
 */

#ifndef CPPTESTLINTDOC_EXCEPTIONS_H
#define CPPTESTLINTDOC_EXCEPTIONS_H

#include <iostream>
#include <utility>
#include <string>
#include <string_view>

namespace Exceptions {
    /*!
     * @brief just calling an exception with the given description
     */
    class ExceptionWithDescription: public std::exception {
    private:

        std::string message;

    public:

        /*!
         * @brief constructor that helps to give more descriptive errors. Just use this after "throw" with string you want to put to console
         *
         * @param message a string to show
         */
        explicit ExceptionWithDescription(std::string message): message(std::move(message)) {}

        [[nodiscard]] char const *what() const noexcept override;

    };
}

#endif //CPPTESTLINTDOC_EXCEPTIONS_H
