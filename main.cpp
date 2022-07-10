#include <iostream>

#include "Interpreter.h"

/**
 *                  DOCUMENTATION
 *
 * Interpreter for a functional language.
 *
 * Use "exit" (spelled all lowercase) to exit the interpreter.
 *
 * Before running the program you can specify files to be loaded
 * using the command line arguments. All correct definitions will
 * be loaded and can be used in the interpreter.
 *
 * In the interpreter you can define and evaluate your own functions.
 *
 * There is a number of built in functions, which are:
 *
 *      - eq(#0, #1)
 *      - le(#0, #1)
 *      - nand(#0, #1)
 *      - length(#0)
 *      - head(#0)
 *      - tail(#0)
 *      - list(#0)
 *      - list2(#0, #1)
 *      - list3(#0, #1, #2)
 *      - concat(#0, #1)
 *      - if(#0, #1, #2)
 *      - read()
 *      - write(#0)
 *      - int(#0)
 *
 *      - add(#0, #1)
 *      - sub(#0, #1)
 *      - mul(#0, #1)
 *      - div(#0, #1)
 *      - mod(#0, #1)
 *      - sqrt(#0)
 *
 * Also this language supports the operators:
 *      - '+' (Binary and Unary)
 *      - '-' (Binary and Unary)
 *      - '*'
 *      - '/'
 *      - '%'
 *      - '^'
 *
 * The constants PI and E are also available (MUST BE spelled with upper case letters).
 *
 * There are only two types of literals:
 *      - Number (double).
 *      - List.
 *
 * Comments are supported. Every line beginning with '//'
 * will be treated as a comment. Comments are only allowed outside function
 * definitions.
 *
 * ! WHEN YOU DECLARE LISTS PLEASE PUT A COMMA (',') BETWEEN THE ELEMENTS !
 * !               OR IT WILL NOT BE EVALUATED CORRECTLY                  !
 *
 * Some rules of the language:
 *      - The List literal can contain either numbers or other lists or both;
 *      - Lists and Numbers both can be used as Boolean Values:
 *          - 0.0 == False / Every other number == True
 *          - The empty list ([]) == False / List with at least one element == True.
 *      - Parenthesis follow immediately after each function call,
 *        no whitespaces allowed.
 *      - Each argument should be separated by a comma (',').
 *
 *
 *  ------------------------------------------------------------------------------------
 *  |     __   __    ___   __     __   ______       ______  __   __   ___  __     _    |
 *  |    | |__| |   / __\  \ \   / /  | ____|      | ____| | |  | |  |  \ | |    ||    |
 *  |    |  __  |  /  __ \  \ \ / /   | __|        | __|   | |__| |  | |\\| |    ||    |
 *  |    |_|  |_| /_/   \_\  \___/    |_____|      |_|     |______|  |_| \__|    v     |
 *  |                                                                                  |
 *  ------------------------------------------------------------------------------------
 */

int main(int argc, char** argv)
{
    Interpreter interpreter(argv + 1, argc - 1);

    interpreter.run();

    return 0;
}