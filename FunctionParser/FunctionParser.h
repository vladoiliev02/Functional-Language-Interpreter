#pragma once

#include <iostream>
#include <string>

#include "Expression.h"
#include "exit_exception.h"


/**
 * @brief - Handles the users input and parses the function definitions
 *          and loads the im the symbol table
 */
class FunctionParser {

    static inline const char* INVALID_FUNCTION_DEFINITION = "Invalid function definition in line:\n";
    static inline const char* INVALID_PARAMETER = "Invalid parameter in line:\n";
    static inline const char* EXIT_COMMAND = "exit";
    static inline int EXIT_COMMAND_LENGTH = 4;

    SymbolTable& symbol_table;
private:
    static bool is_valid_name_letter(char c);
    static bool letter(char c);
    static bool digit(char c);
    static bool is_function_definition(const std::string& line);
    static void skip_blank_spaces(const std::string& line, std::size_t& i);
    static bool is_blanc(const std::string& line, std::size_t& i);
    static bool should_exit(const std::string& line);

public:
    FunctionParser(SymbolTable& symbol_table);


    friend std::istream& operator>>(std::istream& is, FunctionParser& fp);
};



