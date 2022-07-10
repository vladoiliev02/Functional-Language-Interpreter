#pragma once

#include <string>
#include <stack>
#include <unordered_map>
#include <vector>

#include "Literal.h"
#include "SymbolTable.h"
#include "StackFrame.h"

class Expression {

    using mem_func_ptr = void (Expression::*)();

    static inline const char* INVALID_EXPRESSION = "Expression :: Invalid expression in expression: ";
    static inline const char* UNKNOWN_OPERATOR = "Expression :: Unknown operator in expression: ";
    static inline const char* UNKNOWN_FUNCTION_OR_CONSTANT = "Expression :: Unknown function or constant in expression: ";
    static inline const char* INVALID_BRACKETS = "Expression :: Invalid brackets in expression: ";

    /// Maps each closing bracket to its opening one.
    static const std::unordered_map<char, char> opening_bracket;

    /// Maps each operator to its precedence and num of arguments and function.
    static const std::unordered_map<std::string, std::tuple<int, double, mem_func_ptr>> ops;


    SymbolTable& symbol_table;
    StackFrame* stack_frame;

    std::string expression;     /// The expression.
    std::size_t i = 0;          /// Position in the expression.
    std::size_t len = 0;        /// Length of the expression.

    std::stack<Literal*> value_stack{};
    std::stack<std::string> op_stack{};

    std::vector<Literal*> args{};

private:

    /**
     * Utility functions checking if the argument is valid.
     */

    static bool is_digit(char c);
    static bool is_opening_bracket(char c);
    static bool is_closing_bracket(char c);
    static bool is_left_associative(const std::string& op);
    bool is_operator(const std::string& op);
    static bool is_bracket(char c);
    bool is_function(const std::string& op);
    static bool is_letter(char c);
    void get_arguments(const std::string& op);
    void determine_variadic_func(std::string& op, short min_num_args);
    static bool is_integer(double a);

    /**
     * @brief Parses the numbers in the expression
     *
     * @returns the current number
     */
    Literal* parse_number();
    Literal* parse_list();

    /**
     * @brief Takes an operator, then takes the arguments from
     *        the @p value_stack and evaluates the expression.
     *        In the end it pushes the new value on the @p value_stack.
     * @param op - The operator to be applied.
     */
    void evaluate(const std::string& func);

    /**
     * @brief Takes the next operator, pops previous ones off the
     *        @p op_stack and pushes the new op.
     *
     * @param op - The new operator.
     */
    void handle_operator(std::string& op);

    /// Built in functions
    double eq(const Literal& a, const Literal& b);
    double le(double a, double b);
    double read();
    double write(Literal* a);
    void _if();
    void nand();

    std::string get_argument_expr();
    std::size_t skip_argument_expr();

    void execute_add();
    void execute_sub();
    void execute_mul();
    void execute_div();
    void execute_mod();
    void execute_sqrt();
    void execute_pow();
    void execute_unary_plus();
    void execute_unary_minus();
    void execute_comma();
    void execute_eq();
    void execute_le();
    void execute_length();
    void execute_head();
    void execute_tail();
    void execute_list();
    void execute_list2();
    void execute_list3();
    void execute_concat();
    void execute_read();
    void execute_write();
    void execute_int();


    void get_function_parameter();

public:
    Expression(std::string expression, SymbolTable& symbol_table, StackFrame* stack_frame = nullptr);

    /**
     * @brief Calculates the expression.
     *
     * @returns The value of the expression
     */
    Literal* calculate();
};