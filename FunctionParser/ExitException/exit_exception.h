#pragma once

#include <stdexcept>

/**
 * @brief - This exception is thrown when the user
 *          types "exit" when they want to exit the interpreter
 */
class exit_exception : public std::exception {
    const char* message;

public:
    explicit exit_exception(const char* message);

    const char* what() const noexcept override;
};



