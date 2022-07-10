#pragma once

#include <unordered_map>
#include <string>

#include <unordered_set>

/**
 * @brief - Class representing a symbol table containing all
 *          user defined names and their definitions and number of arguments.
 */
class SymbolTable {
    static const std::unordered_set<std::string> reserved_words;

    std::unordered_map<std::string, std::pair<int ,std::string>> functions;

private:
    static bool is_reserved(const std::string& name);

public:

    void add_definition(const std::string& name, const std::pair<int, std::string>& definition);
    bool contains(const std::string& name) const;
    const std::pair<int, std::string>& at(const std::string& name) const;
};



