/*!
 *  @author Ailurus
 */

#include "Exceptions.h"

char const *Exceptions::ExceptionWithDescription::what() const noexcept {
    return message.c_str();
}