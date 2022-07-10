#include <cassert>
#include "StackFrame.h"
#include "Expression.h"

StackFrame::StackFrame(const std::string& body, const std::vector<Literal*>& arguments, SymbolTable& symbol_table)
        : body(body),
          symbol_table(symbol_table)
{
    for (Literal* arg: arguments) {
        if (arg->get_type() == LITERAL_TYPE::LIST)
            this->arguments.push_back(new List(arg->to_list()));
        else
            this->arguments.push_back(new Double(arg->get_double()));
    }
}

Literal* StackFrame::evaluate()
{
    return Expression(body, symbol_table, this).calculate();
}

StackFrame::~StackFrame()
{
    for (Literal* arg: arguments)
        delete arg;
}

Literal* StackFrame::get_argument(int idx)
{
    assert(idx < arguments.size());

    Literal* lit = arguments[idx];

    if (lit->get_type() == LITERAL_TYPE::LIST)
        return new List(std::move(lit->to_list()));

    return new Double(lit->get_double());
}
