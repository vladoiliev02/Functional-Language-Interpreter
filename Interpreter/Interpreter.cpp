#include "Interpreter.h"

#include <iostream>

Interpreter::Interpreter(char** paths, int num_of_paths)
{
    for (int i = 0; i < num_of_paths; ++i) {
        std::ifstream ifs(paths[i]);

        while (!ifs.eof()) {
            try {
                ifs >> function_interpreter;
            } catch (std::exception& e) {
                std::cerr << e.what() << '\n';
            }
        }
    }
}

void Interpreter::run()
{
    while (true) {
        try {
            std::cin >> function_interpreter;
        } catch (exit_exception& e) {
            std::cout << e.what() << '\n';
            return;
        } catch (std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }
}
