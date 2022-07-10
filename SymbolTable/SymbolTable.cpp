#include <stdexcept>
#include "SymbolTable.h"

const std::unordered_set<std::string> SymbolTable::reserved_words{
        "+",
        "-",
        "*",
        "/",
        "%",
        "^",
        "+u",
        "-u",
        ",",
        "eq",
        "le",
        "nand",
        "length",
        "head",
        "tail",
        "list",
        "list2",
        "list3",
        "concat",
        "if",
        "read",
        "write",
        "int",
        "add",
        "sub",
        "mul",
        "div",
        "mod",
        "sqrt",
        "PI",
        "E",
};

void SymbolTable::add_definition(const std::string& name, const std::pair<int, std::string>& definition)
{
    if (is_reserved(name))
        throw std::invalid_argument(name + " is a reserved identifier.");

    functions[name] = definition;
}

bool SymbolTable::contains(const std::string& name) const
{
    if (is_reserved(name))
        throw std::invalid_argument(name + " is a reserved identifier.");

    return functions.contains(name);
}

const std::pair<int, std::string>& SymbolTable::at(const std::string& name) const
{
    if (is_reserved(name))
        throw std::invalid_argument(name + " is a reserved identifier.");

    return functions.at(name);
}

bool SymbolTable::is_reserved(const std::string& name)
{
    return reserved_words.contains(name);
}
