#include "exit_exception.h"

exit_exception::exit_exception(const char* message)
        : message(message)
{}

const char* exit_exception::what() const noexcept
{
    return message;
}