#pragma once

#include <string>
#include <vector>
#include "Literal.h"
#include "SymbolTable.h"

/**
 * @brief - This class represents a single stack frame containing:
 *              - A symbol table
 *              - The body of the function
 *              - The number of arguments
 *              - The actual arguments.
 */
class StackFrame {
    SymbolTable& symbol_table;
    std::string body;
    std::vector<Literal*> arguments;

public:
    StackFrame(const std::string& body, const std::vector<Literal*>& arguments, SymbolTable& symbol_table);
    ~StackFrame();

    /**
     * @brief Evaluates the body of the the function
     *        using the arguments passed to it
     *
     * @returns The value of the function
     */
    Literal* evaluate();

    /**
     * @returns - The argument corresponding the @p idx int the @ p arguments vector
     */
    Literal* get_argument(int idx);
};



