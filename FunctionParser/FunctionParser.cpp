#include "FunctionParser.h"

#include <stack>
#include <set>

std::istream& operator>>(std::istream& is, FunctionParser& fp)
{
    std::string line;
    std::getline(is, line);

    if (FunctionParser::should_exit(line))
        throw exit_exception("> Goodbye! :)");

    std::size_t i = 0;

    if (FunctionParser::is_blanc(line, i))
        return is;

    if (line.length() > 1 && line[0] == '/' && line[1] == '/')
        return is;

    std::string expression;
    std::string function_name;

    std::stack<char> brackets;

    if (FunctionParser::is_function_definition(line)) {
        /// Parse function name

        std::size_t size = line.length();
        FunctionParser::skip_blank_spaces(line, i);
        for (; i < size && line[i] != ' '; ++i) {
            if (FunctionParser::is_valid_name_letter(line[i])) {
                function_name.push_back(line[i]);
            } else {
                throw std::invalid_argument(FunctionParser::INVALID_FUNCTION_DEFINITION + line);
            }
        }
        FunctionParser::skip_blank_spaces(line, i);
        if (i >= size || i + 1 >= size || line[i] != '-' || line[i + 1] != '>')
            throw std::invalid_argument(FunctionParser::INVALID_FUNCTION_DEFINITION + line);

        i += 2;

        /// Parse function body
        std::set<std::string> parameters;

        while (!brackets.empty() || expression.empty()) {
            if (i >= size) {
                std::getline(is, line);
                size = line.length();
                i = 0;
            }
            while (i < size) {
                if (line[i] == '#') {
                    std::string arg;
                    expression.push_back(line[i++]);
                    while (i < size && FunctionParser::digit(line[i])) {
                        expression.push_back(line[i]);
                        arg.push_back(line[i++]);
                    }

                    if (arg.empty())
                        throw std::invalid_argument(FunctionParser::INVALID_PARAMETER + line);

                    parameters.insert(arg);
                } else if (line[i] == '(' || line[i] == '[') {
                    brackets.push(line[i]);
                    expression.push_back(line[i++]);
                } else if (line[i] == ')' || line[i] == ']') {
                    if (line[i] == '(' && brackets.top() != ')' || line[i] == '[' && brackets.top() != ']')
                        throw std::invalid_argument("Invalid brackets in line:\n" + line);
                    brackets.pop();
                    expression.push_back(line[i++]);
                } else {
                    expression.push_back(line[i++]);
                }
            }
        }

        if (!fp.symbol_table.contains(function_name))
            std::cout << "> 0\n";
        else
            std::cout << "> 1\n";

        fp.symbol_table.add_definition(function_name, std::make_pair(parameters.size(), expression));
    } else {
        for (char c : line)
            expression.push_back(c);

        Literal* result = Expression(expression, fp.symbol_table).calculate();
        std::cout << "> " << *result << '\n';
        delete result;
    }

    return is;
}

FunctionParser::FunctionParser(SymbolTable& symbol_table)
    : symbol_table(symbol_table)
{}

bool FunctionParser::is_function_definition(const std::string& line)
{
    std::size_t size = line.length();
    for (std::size_t i = 0; i < size; ++i)
        if (line[i] == '-' && i + 1 < size && line[i + 1] == '>')
            return true;

    return false;
}

bool FunctionParser::is_valid_name_letter(char c)
{
    return letter(c) || digit(c);
}

bool FunctionParser::letter(char c)
{
    return 'a' <= c && c <= 'z' ||
           'A' <= c && c <= 'Z';
}

bool FunctionParser::digit(char c)
{
    return '0' <= c && c <= '9';
}

void FunctionParser::skip_blank_spaces(const std::string& line, std::size_t& i)
{
    std::size_t length = line.length();
    while (i < length && (line[i] == ' ' || line[i] == '\t'))
        ++i;
}

bool FunctionParser::is_blanc(const std::string& line, size_t& i)
{
    skip_blank_spaces(line, i);
    return i == line.length();
}

bool FunctionParser::should_exit(const std::string& line)
{
    std::size_t j = 0;
    skip_blank_spaces(line, j);

    int i = 0;
    std::size_t len = line.length();

    for (;i < EXIT_COMMAND_LENGTH && j < line.length(); ++j)
        if (tolower(line[j]) != EXIT_COMMAND[i++])
            return false;

    return j == line.size() && i == EXIT_COMMAND_LENGTH;
}