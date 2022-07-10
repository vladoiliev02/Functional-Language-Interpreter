#include "Expression.h"

#include <stdexcept>
#include <cmath>
#include <cassert>

const std::unordered_map<std::string, std::tuple<int, double, Expression::mem_func_ptr>> Expression::ops {
        /// Operators / Function -> { Precedence : Num_Args : Function }

        /// Operators
        {"+",      std::make_tuple(2, 2, &Expression::execute_add)},
        {"-",      std::make_tuple(2, 2, &Expression::execute_sub)},
        {"*",      std::make_tuple(3, 2, &Expression::execute_mul)},
        {"/",      std::make_tuple(3, 2, &Expression::execute_div)},
        {"%",      std::make_tuple(3, 2, &Expression::execute_mod)},
        {"^",      std::make_tuple(4, 2, &Expression::execute_pow)},
        {"+u",     std::make_tuple(6, 1, &Expression::execute_unary_plus)}, /// unary plus
        {"-u",     std::make_tuple(6, 1, &Expression::execute_unary_minus)}, /// unary minus
        {",",      std::make_tuple(-2, 0, &Expression::execute_comma)},

        /// Functions - { 0 : Num_Args : Function }
        {"eq",     std::make_tuple(0, 2, &Expression::execute_eq)},
        {"le",     std::make_tuple(0, 2, &Expression::execute_le)},
        {"nand",   std::make_tuple(0, 2, nullptr)},
        {"length", std::make_tuple(0, 1, &Expression::execute_length)},
        {"head",   std::make_tuple(0, 1, &Expression::execute_head)},
        {"tail",   std::make_tuple(0, 1, &Expression::execute_tail)},
        {"list",   std::make_tuple(0, 1, &Expression::execute_list)},
        {"list2",  std::make_tuple(0, 2, &Expression::execute_list2)},
        {"list3",  std::make_tuple(0, 3, &Expression::execute_list3)},
        {"concat", std::make_tuple(0, 2, &Expression::execute_concat)},
        {"if",     std::make_tuple(0, 3, nullptr)},
        {"read",   std::make_tuple(0, 0, &Expression::execute_read)},
        {"write",  std::make_tuple(0, 1, &Expression::execute_write)},
        {"int",    std::make_tuple(0, 1, &Expression::execute_int)},

        {"add",    std::make_tuple(0, 2, &Expression::execute_add)},
        {"sub",    std::make_tuple(0, 2, &Expression::execute_sub)},
        {"mul",    std::make_tuple(0, 2, &Expression::execute_mul)},
        {"div",    std::make_tuple(0, 2, &Expression::execute_div)},
        {"mod",    std::make_tuple(0, 2, &Expression::execute_mod)},
        {"sqrt",   std::make_tuple(0, 1, &Expression::execute_sqrt)},

        /// Constants - { -1 : Value }
        {"PI",     std::make_tuple(-1, 3.14159265358979323846, nullptr)},
        {"E",      std::make_tuple(-1, 2.7182818284590452354, nullptr)},
};

const std::unordered_map<char, char> Expression::opening_bracket {
        {')', '('},
        {']', '['},
        {'}', '{'}
};

Expression::Expression(std::string expression, SymbolTable& symbol_table, StackFrame* stack_frame)
        : expression(std::move(expression)),
          len(this->expression.length()),
          stack_frame(stack_frame),
          symbol_table(symbol_table)
{
    if (this->expression.empty())
        throw std::invalid_argument(INVALID_EXPRESSION + std::string("Empty expression"));
}

bool Expression::is_left_associative(const std::string& op)
{
    return op != "^";
}

bool Expression::is_digit(char c)
{
    return '0' <= c && c <= '9';
}

Literal* Expression::parse_number()
{
    std::string result{};

    while (i < len && is_digit(expression[i]))
        result.push_back(expression[i++]);

    if (expression[i] == '.') {
        result.push_back(expression[i++]);
        while (i < len && is_digit(expression[i]))
            result.push_back(expression[i++]);
    }

    return new Double(std::stod(result));
}

bool Expression::is_opening_bracket(char c)
{
    return c == '(' || c == '[' || c == '{';
}

bool Expression::is_closing_bracket(char c)
{
    return c == ')' || c == ']' || c == '}';
}

Literal* Expression::calculate()
{
    while (i < len) {
        while (i < len && expression[i] == ' ' || expression[i] == '\t')
            ++i;
        if (i < len && is_digit(expression[i])) { /// is number literal
            value_stack.push(parse_number());
        } else if (i < len && expression[i] == '[') { /// is list literal
            value_stack.push(parse_list());
        } else if (i < len && is_opening_bracket(expression[i])) {
            op_stack.push({expression[i++]});
        } else if (i < len && is_closing_bracket(expression[i])) {
            char matching_br = opening_bracket.at(expression[i++]);

            while (!op_stack.empty() && op_stack.top().front() != matching_br) {
                evaluate(op_stack.top());
                op_stack.pop();
            }

            if (op_stack.empty() || op_stack.top().front() != matching_br)
                throw std::invalid_argument(INVALID_BRACKETS + expression);

            op_stack.pop();

            if (!op_stack.empty() && is_function(op_stack.top())) {
                evaluate(op_stack.top());
                op_stack.pop();
            }
        } else if (i < len && is_letter(expression[i])) { /// is function or constant
            std::string f;

            while (i < len && (is_letter(expression[i]) || is_digit(expression[i])))
                f.push_back(expression[i++]);

            try {
                const auto&[precedence, value, func] = ops.at(f);
                switch (precedence) {

                    case 0 : { /// is function
                        if (i >= len || i < len && expression[i] != '(')
                            throw std::invalid_argument(INVALID_BRACKETS + expression + "\nMissing parenthesis in function call to " + f);

                        if (f == "list")
                            determine_variadic_func(f, 1);
                        else if (f == "if") {
                            _if();
                            break;
                        } else if (f == "nand") {
                            nand();
                            break;
                        }

                        op_stack.push(f);
                        break;
                    }
                    case -1: { /// is constant
                        value_stack.push(new Double(value));
                        break;
                    }
                    default:
                        throw std::invalid_argument(UNKNOWN_FUNCTION_OR_CONSTANT + expression + "\ngiven: " + f);
                }
            } catch (std::out_of_range&) {
                if (i >= len || i < len && expression[i] != '(')
                    throw std::invalid_argument(INVALID_BRACKETS + expression + "\nMissing parenthesis in function call to " + f);

                if (symbol_table.contains(f))/// User defined func maybe
                    op_stack.push(f);
                else
                    throw std::invalid_argument(UNKNOWN_FUNCTION_OR_CONSTANT + expression + "\ngiven: " + f);
            }

        } else if (i < len && expression[i] == '#') {
            get_function_parameter();
        } else { /// op
            std::string op = {expression[i++]};

            handle_operator(op);
        }
    }

    while (!op_stack.empty()) {
        evaluate(op_stack.top());
        op_stack.pop();
    }

    Literal* res;

    if (value_stack.top()->get_type() == LITERAL_TYPE::DOUBLE)
        res = new Double(value_stack.top()->get_double());
    else
        res = new List(std::move(value_stack.top()->to_list()));

    delete value_stack.top();
    value_stack.pop();

    if (!value_stack.empty())
        throw std::invalid_argument(INVALID_EXPRESSION + expression);

    return res;
}

void Expression::evaluate(const std::string& op)
{
    get_arguments(op);

    if (ops.contains(op)) {
        (this->*std::get<2>(ops.at(op)))();
    } else if (symbol_table.contains(op)) {
        auto&[num_args, body] = symbol_table.at(op);
        value_stack.push(StackFrame(body, args, symbol_table).evaluate());
    } else {
        throw std::invalid_argument(UNKNOWN_OPERATOR + expression + "\ngiven " + op);
    }

    for (Literal* arg: args)
        delete arg;
}

void Expression::handle_operator(std::string& op)
{
    if (op == "-") {
        int j = i - 2;
        while (j >= 0 && expression[j] == ' ')
            --j;

        if (j < 0 || expression[j] == '(' || is_operator({expression[j]}) || expression[j] == ',')
            op = "-u";
    } else if (op == "+") {
        int j = i - 2;
        while (j >= 0 && expression[j] == ' ')
            --j;

        if (j < 0 || expression[j] == '(' || is_operator({expression[j]}) || expression[j] == ',')
            op = "+u";
    }

    int precedence;
    try {
        precedence = std::get<0>(ops.at(op));
    } catch (std::out_of_range&) {
        throw std::invalid_argument(UNKNOWN_OPERATOR + expression);
    }

    if (is_left_associative(op)) {
        while (!op_stack.empty()) {
            std::string cur_op = op_stack.top();
            if (!is_opening_bracket(cur_op[0]) && precedence <= std::get<0>(ops.at(cur_op))) {
                evaluate(op_stack.top());
                op_stack.pop();
            } else {
                break;
            }
        }
    } else { /// is right associative
        while (!op_stack.empty()) {
            std::string cur_op = op_stack.top();
            if (!is_opening_bracket(cur_op[0]) && precedence < std::get<0>(ops.at(cur_op))) {
                evaluate(op_stack.top());
                op_stack.pop();
            } else {
                break;
            }
        }
    }

    op_stack.push(op);
}

bool Expression::is_operator(const std::string& op)
{
    return ops.contains(op);
}

bool Expression::is_bracket(char c)
{
    return is_opening_bracket(c) || is_closing_bracket(c);
}

bool Expression::is_function(const std::string& op)
{
    try {
        return std::get<0>(ops.at(op)) == 0;
    } catch (std::out_of_range&) {
        return symbol_table.contains(op);
    }
}

bool Expression::is_letter(char c)
{
    return 'a' <= c && c <= 'z' ||
           'A' <= c && c <= 'Z';
}

void Expression::get_arguments(const std::string& op)
{
    short num_args;
    args.clear();

    try {
        num_args = (short) std::get<1>(ops.at(op_stack.top()));
    } catch (std::out_of_range&) {
        try {
            num_args = (short) symbol_table.at(op).first;
        } catch (std::out_of_range&) {
            throw std::invalid_argument(UNKNOWN_OPERATOR + expression + "\n given " + op);
        }
    }

    args.resize(num_args);

    for (int j = num_args - 1; j >= 0; --j) {
        if (value_stack.empty())
            throw std::invalid_argument(INVALID_EXPRESSION + expression);

        args[j] = value_stack.top();
        value_stack.pop();
    }
}

void Expression::determine_variadic_func(std::string& op, short min_num_args)
{
    short num_args = 0;

    std::stack<char> brackets;

    std::size_t j = i;
    while (j < len && expression[j] != '(')
        ++j;

    if (j < len && expression[j] == '(')
        ++j;
    else
        throw std::invalid_argument(INVALID_BRACKETS + expression);

    while (j < len && expression[j] != ')') {
        if (expression[j] == '(' || expression[j] == '[') {
            brackets.push(expression[j++]);
            while (j < len && !brackets.empty()) {
                if (expression[j] == '(' || expression[j] == '[')
                    brackets.push(expression[j++]);
                else if (expression[j] == ')' || expression[j] == ']') {
                    if (brackets.top() != opening_bracket.at(expression[j]))
                        throw std::invalid_argument(INVALID_BRACKETS + expression);
                    brackets.pop();
                    ++j;
                } else {
                    ++j;
                }
            }
        }
        if (expression[j++] == ',')
            num_args++;
    }

    if (num_args != 0)
        num_args++;

    num_args = std::max(min_num_args, num_args);

    op += num_args == min_num_args ? "" : std::to_string(num_args);
}

double Expression::eq(const Literal& a, const Literal& b)
{
    return a == b ? 1.0 : 0.0;
}

double Expression::le(double a, double b)
{
    return a < b ? 1.0 : 0.0;
}

double Expression::read()
{
    double n;
    std::cin >> n;
    return n;
}

double Expression::write(Literal* a)
{
    assert(a);
    try {
        std::cout << *a << '\n';
        return 0;
    } catch (...) {
        return 1;
    }
}

Literal* Expression::parse_list()
{
    std::list<Literal*> lst;
    if (expression[i] == '[') {
        ++i;
        while (i < len && expression[i] != ']') {
            if (expression[i] == ' ' || expression[i] == ',') {
                ++i;
                continue;
            } else if (is_digit(expression[i])) {
                Literal* num = parse_number();
                lst.push_back(new Double(num->get_double()));
                delete num;
            } else if (is_letter(expression[i])) {
                std::string expr = get_argument_expr();
                Literal* value = Expression(expr, symbol_table, stack_frame).calculate();
                lst.push_back(value);
            } else if (expression[i] == '[') {
                lst.push_back(parse_list());
            } else {
                throw std::invalid_argument(INVALID_EXPRESSION + expression);
            }
        }
    }

    if (i >= len || expression[i] != ']')
        throw std::invalid_argument(INVALID_BRACKETS + expression);

    ++i;

    return new List(lst);
}

void Expression::_if()
{
    while (i < len && expression[i] == ' ')
        ++i;

    if (i< len && expression[i] == '(') {
        ++i;
        std::string arg = get_argument_expr();

        Literal* predicate = Expression(arg, symbol_table, stack_frame).calculate();

        std::size_t skipped;
        if (*predicate) {
            arg = get_argument_expr();
            value_stack.push(Expression(arg, symbol_table, stack_frame).calculate());
            skipped = skip_argument_expr();
        } else {
            skipped = skip_argument_expr();
            arg = get_argument_expr();
            value_stack.push(Expression(arg, symbol_table, stack_frame).calculate());
        }

        if (skipped == 0)
            throw std::invalid_argument(INVALID_EXPRESSION + expression);

        delete predicate;
    }

    if (i >= len || expression[i] != ')')
        throw std::invalid_argument(INVALID_BRACKETS + expression);

    ++i;
}


void Expression::nand()
{
    while (i < len && expression[i] == ' ')
        ++i;

    if (i< len && expression[i] == '(') {
        ++i;
        std::string arg = get_argument_expr();

        Literal* arg_expr = Expression(arg, symbol_table, stack_frame).calculate();

        if (*arg_expr) {
            arg = get_argument_expr();
            value_stack.push(*Expression(arg, symbol_table, stack_frame).calculate()
                             ? new Double(0.0) : new Double(1.0));
        } else {
            std::size_t skipped = skip_argument_expr();

            if (skipped == 0)
                throw std::invalid_argument(INVALID_EXPRESSION + expression);

            value_stack.push(new Double(1.0));
        }

        delete arg_expr;
    }

    if (i >= len || expression[i] != ')')
        throw std::invalid_argument(INVALID_BRACKETS + expression);

    ++i;
}


std::string Expression::get_argument_expr()
{
    std::string expr;
    std::stack<char> bracket_stack;

    while (i < len && expression[i] != ',' && expression[i] != ')' && expression[i] != ']') {
        if (expression[i] == '(' || expression[i] == '[') {
            bracket_stack.push(expression[i]);
            expr.push_back(expression[i++]);
            while (i < len && !bracket_stack.empty()) {
                if (expression[i] == '(' || expression[i] == '[') {
                    bracket_stack.push(expression[i]);
                    expr.push_back(expression[i++]);
                } else if (expression[i] == ')'  || expression[i] == ']') {
                    if (bracket_stack.top() != opening_bracket.at(expression[i]))
                        throw std::invalid_argument(INVALID_BRACKETS + expression);
                    bracket_stack.pop();
                    expr.push_back(expression[i++]);
                } else {
                    expr.push_back(expression[i++]);
                }
            }

            if (!bracket_stack.empty())
                throw std::invalid_argument(INVALID_EXPRESSION + expression);
        } else {
            expr.push_back(expression[i++]);
        }
    }

    if (i >= len || i < len && expression[i] != ',' && expression[i] != ')' && expression[i] != ']' && expression[i] != ' ')
        throw std::invalid_argument(INVALID_EXPRESSION + expression);

    if (i < len && expression[i] == ',')
        ++i;

    while (i < len && expression[i] == ' ')
        ++i;

    return expr;
}

std::size_t Expression::skip_argument_expr()
{
    std::size_t j = i;
    std::stack<char> bracket_stack;

    while (i < len && expression[i] != ',' && expression[i] != ')' && expression[i] != ']') {
        if (expression[i] == '(' || expression[i] == '[') {
            bracket_stack.push(expression[i]);
            i++;
            while (i < len && !bracket_stack.empty()) {
                if (expression[i] == '(' || expression[i] == '[') {
                    bracket_stack.push(expression[i]);
                    i++;
                } else if (expression[i] == ')' || expression[i] == ']') {
                    if (bracket_stack.top() != opening_bracket.at(expression[i]))
                        throw std::invalid_argument(INVALID_BRACKETS + expression);
                    bracket_stack.pop();
                    i++;
                } else {
                    i++;
                }
            }

            if (!bracket_stack.empty())
                throw std::invalid_argument(INVALID_BRACKETS + expression);
        } else {
            i++;
        }
    }

    if (i >= len || i < len && expression[i] != ',' && expression[i] != ')' && expression[i] != ']' && expression[i] != ' ')
        throw std::invalid_argument(INVALID_EXPRESSION + expression);

    if (i < len && expression[i] == ',')
        ++i;

    while (i < len && expression[i] == ' ')
        ++i;

    return i - j;
}

void Expression::execute_add()
{
    value_stack.push(new Double(args[0]->get_double() + args[1]->get_double()));
}

void Expression::execute_sub()
{
    value_stack.push(new Double(args[0]->get_double() - args[1]->get_double()));
}

void Expression::execute_mul()
{
    value_stack.push(new Double(args[0]->get_double() * args[1]->get_double()));
}

void Expression::execute_div()
{
    value_stack.push(new Double(args[0]->get_double() / args[1]->get_double()));
}

void Expression::execute_mod()
{
    if (!is_integer(args[0]->get_double())) {
        throw std::invalid_argument("Invalid first argument of modulo operation in expression: " +
                                    expression +
                                    "\nExpected integer, actual type is double.");
    }

    if (!is_integer(args[1]->get_double())) {
        throw std::invalid_argument("Invalid second argument of modulo operation in expression: " +
                                    expression +
                                    "\nExpected integer, actual type is double.");
    }

    value_stack.push(new Double((int) args[0]->get_double() % (int) args[1]->get_double()));
}

void Expression::execute_sqrt()
{
    value_stack.push(new Double(std::sqrt(args[0]->get_double())));
}

void Expression::execute_pow()
{
    value_stack.push(new Double(std::pow(args[0]->get_double(), args[1]->get_double())));
}

void Expression::execute_unary_plus()
{
    value_stack.push(new Double(args[0]->get_double()));
}

void Expression::execute_unary_minus()
{
    value_stack.push(new Double((-1) * args[0]->get_double()));
}

void Expression::execute_comma()
{
    /**
    * Do nothing
    * The comma operator only causes the expressions before it to be evaluated
    */
    return;
}

void Expression::execute_eq()
{
    value_stack.push(new Double(eq(args[0]->to_list(), args[1]->to_list())));
}

void Expression::execute_le()
{
    value_stack.push(new Double(le(args[0]->get_double(), args[1]->get_double())));
}

void Expression::execute_length()
{
    value_stack.push(new Double(args[0]->length()));
}

void Expression::execute_head()
{
    const Literal* head = args[0]->head();
    if (head->get_type() == LITERAL_TYPE::LIST)
        value_stack.push(new List(head->to_list()));
    else
        value_stack.push(new Double(head->get_double()));
}

void Expression::execute_tail()
{
    value_stack.push(new List(args[0]->tail()));
}

void Expression::execute_list()
{
    value_stack.push(new List(args[0]->get_double()));
}

void Expression::execute_list2()
{
    value_stack.push(new List(args[0]->get_double(), (int) args[1]->get_double()));
}

void Expression::execute_list3()
{
    value_stack.push(new List(args[0]->get_double(), args[1]->get_double(), (int) args[2]->get_double()));
}

void Expression::execute_concat()
{
    value_stack.push(new List(args[0]->to_list().concat(args[1]->to_list())));
}

void Expression::execute_read()
{
    value_stack.push(new Double(read()));
}

void Expression::execute_write()
{
    value_stack.push(new Double(write(args[0])));
}

void Expression::execute_int()
{
    value_stack.push(new Double(std::floor(args[0]->get_double())));
}

void Expression::get_function_parameter()
{
    if (expression[i] == '#') {
        ++i;

        std::string num;
        while (i < len && is_digit(expression[i]))
            num.push_back(expression[i++]);

        int num_arg = std::strtol(num.c_str(), nullptr, 10);
        assert(stack_frame);

        value_stack.push(stack_frame->get_argument(num_arg));
    }
}

bool Expression::is_integer(double a)
{
    return std::abs(a - std::floor(a)) < 0.0001;
}
