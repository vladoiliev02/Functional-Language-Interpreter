#pragma once

#include <fstream>
#include "SymbolTable.h"
#include "FunctionParser.h"


/**
 * @brief - The interpreter
 */
class Interpreter {

    SymbolTable global_symbol_table{};
    FunctionParser function_interpreter{global_symbol_table};

public:

    Interpreter() = default;

    /**
     * @param paths - the paths to the files from which definitions will be loaded.
     * @param num_of_paths - the number of files.
     *
     * @brief - Loads definitions from files.
     */
    Interpreter(char** paths, int num_of_paths);

    Interpreter(const Interpreter&) = delete;
    Interpreter(Interpreter&&) = delete;
    Interpreter& operator=(const Interpreter&) = delete;
    Interpreter& operator=(Interpreter&&) = delete;

    ~Interpreter() = default;

    /**
     * @brief Runs the interpreter.
     */
    void run();
};



