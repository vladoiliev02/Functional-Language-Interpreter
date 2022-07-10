#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "Expression.h"

TEST_CASE("Expression add")
{
    SymbolTable symbolTable;
    Expression expr("add(1,2)", symbolTable);
    Expression expr2("add(1,2,3)", symbolTable);
    Expression expr3("add(1)", symbolTable);

    Literal* result = expr.calculate();
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(3));

    delete result;
}

TEST_CASE("Expression sub")
{
    SymbolTable symbolTable;
    Expression expr("sub(1,2)", symbolTable);
    Expression expr2("sub(1,2,3)", symbolTable);
    Expression expr3("sub(1)", symbolTable);

    Literal* result = expr.calculate();
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(-1));

    delete result;
}

TEST_CASE("Expression mul")
{
    SymbolTable symbolTable;
    Expression expr("mul(1,2)", symbolTable);
    Expression expr2("mul(1,2,3)", symbolTable);
    Expression expr3("mul(1)", symbolTable);

    Literal* result = expr.calculate();
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(2));

    delete result;
}

TEST_CASE("Expression div")
{
    SymbolTable symbolTable;
    Expression expr("div(1,2)", symbolTable);
    Expression expr2("div(1,2,3)", symbolTable);
    Expression expr3("div(2)", symbolTable);

    Literal* result = expr.calculate();
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(0.5));

    delete result;
}

TEST_CASE("Expression mod")
{
    SymbolTable symbolTable;
    Expression expr("mod(3,2)", symbolTable);
    Expression expr1("mod(4,2)", symbolTable);
    Expression expr3("mod(4,2,5)", symbolTable);
    Expression expr4("mod(4)", symbolTable);
    Expression expr5("mod(4.2, 2)", symbolTable);
    Expression expr6("mod(4, 2.2)", symbolTable);

    Literal* result = expr.calculate();
    Literal* result1 = expr1.calculate();
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr4.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr5.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr6.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(1));
    REQUIRE(*result1 == Double(0));

    delete result;
    delete result1;
}

TEST_CASE("Expression sqrt")
{
    SymbolTable symbolTable;
    Expression expr("sqrt(2)", symbolTable);
    Expression expr1("sqrt(4)", symbolTable);
    Expression expr2("sqrt(4,2)", symbolTable);

    Literal* result = expr.calculate();
    Literal* result1 = expr1.calculate();
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(1.41421));
    REQUIRE(*result1 == Double(2));

    delete result;
    delete result1;
}

TEST_CASE("Expression eq")
{
    SymbolTable symbolTable;
    Expression expr("eq(2,2)", symbolTable);
    Expression expr6("eq(2,1)", symbolTable);
    Expression expr1("eq(4,2,3)", symbolTable);
    Expression expr2("eq(4)", symbolTable);

    Expression expr3("eq([1,2,3], list(1,1,3))", symbolTable);
    Expression expr4("eq([1,2,3], [1,2,3,4])", symbolTable);
    Expression expr5("eq([1,2,3], [1,2])", symbolTable);

    Expression expr7("eq([1], 1)", symbolTable);
    Expression expr8("eq([1, 2], 1)", symbolTable);

    Literal* result = expr.calculate();
    Literal* result3 = expr3.calculate();
    Literal* result4 = expr4.calculate();
    Literal* result5 = expr5.calculate();
    Literal* result6 = expr6.calculate();
    Literal* result7 = expr7.calculate();
    Literal* result8 = expr8.calculate();
    REQUIRE_THROWS_AS(expr1.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(1));
    REQUIRE(*result7 == Double(1));
    REQUIRE(*result3 == Double(1));
    REQUIRE(*result6 == Double(0));
    REQUIRE(*result4 == Double(0));
    REQUIRE(*result5 == Double(0));
    REQUIRE(*result8 == Double(0));

    delete result;
    delete result3;
    delete result4;
    delete result5;
    delete result6;
    delete result7;
    delete result8;
}

TEST_CASE("Expression le")
{
    SymbolTable symbolTable;
    Expression expr("le(2,2)", symbolTable);
    Expression expr6("le(2,1)", symbolTable);
    Expression expr9("le(2,3)", symbolTable);
    Expression expr1("le(4,2,3)", symbolTable);
    Expression expr2("le(4)", symbolTable);

    Expression expr3("le(list(1,1,3), list(1,1,3))", symbolTable);
    Expression expr4("le([1,2,3], [1,2,3,4])", symbolTable);
    Expression expr5("le([1,2,3], [1,2])", symbolTable);

    Expression expr7("le([1], 1)", symbolTable);
    Expression expr8("le([1, 2], 1)", symbolTable);

    Literal* result = expr.calculate();
    Literal* result6 = expr6.calculate();
    Literal* result9 = expr9.calculate();
    REQUIRE_THROWS_AS(expr1.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr4.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr5.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr7.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr8.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(0));
    REQUIRE(*result6 == Double(0));
    REQUIRE(*result9 == Double(1));

    delete result;
    delete result6;
}

TEST_CASE("Expression length")
{
    SymbolTable symbolTable;
    Expression expr("length(2)", symbolTable);
    Expression expr1("length(2,1)", symbolTable);
    Expression expr2("length(4,2,3)", symbolTable);
    Expression expr3("length(list(1,1,3))", symbolTable);
    Expression expr4("length([1,2,3,4])", symbolTable);
    Expression expr5("length([])", symbolTable);
    Expression expr6("length([1])", symbolTable);

    Literal* result3 = expr3.calculate();
    Literal* result4 = expr4.calculate();
    Literal* result5 = expr5.calculate();
    Literal* result6 = expr6.calculate();

    REQUIRE_THROWS_AS(expr.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr1.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);

    REQUIRE(*result3 == Double(3));
    REQUIRE(*result4 == Double(4));
    REQUIRE(*result5 == Double(0));
    REQUIRE(*result6 == Double(1));

    delete result3;
    delete result4;
    delete result5;
    delete result6;
}

TEST_CASE("Expression head")
{
    SymbolTable symbolTable;
    Expression expr("head(2)", symbolTable);
    Expression expr1("head(2,1)", symbolTable);
    Expression expr2("head(4,2,3)", symbolTable);
    Expression expr3("head(list(5,1,3))", symbolTable);
    Expression expr4("head([1,2,3,4])", symbolTable);
    Expression expr5("head([])", symbolTable);
    Expression expr6("head([1])", symbolTable);

    Literal* result3 = expr3.calculate();
    Literal* result4 = expr4.calculate();
    Literal* result6 = expr6.calculate();

    REQUIRE_THROWS_AS(expr.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr1.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr5.calculate(), std::invalid_argument);

    REQUIRE(*result3 == Double(5));
    REQUIRE(*result4 == Double(1));
    REQUIRE(*result6 == Double(1));

    delete result3;
    delete result4;
    delete result6;
}

TEST_CASE("Expression tail")
{
    SymbolTable symbolTable;
    Expression expr("tail(2)", symbolTable);
    Expression expr1("tail(2,1)", symbolTable);
    Expression expr2("tail(4,2,3)", symbolTable);
    Expression expr3("tail(list(5,1,3))", symbolTable);
    Expression expr4("tail([1,2,3,4])", symbolTable);
    Expression expr5("tail([])", symbolTable);
    Expression expr6("tail([1])", symbolTable);

    Literal* result3 = expr3.calculate();
    Literal* result4 = expr4.calculate();
    Literal* result6 = expr6.calculate();

    REQUIRE_THROWS_AS(expr.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr1.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr5.calculate(), std::invalid_argument);

    REQUIRE(*result3 == List(List::list_type({new Double(6), new Double(7)})));
    REQUIRE(*result4 == List(List::list_type({new Double(2), new Double(3), new Double(4)})));
    REQUIRE(*result6 == List());

    delete result3;
    delete result4;
    delete result6;
}

TEST_CASE("Expression list")
{
    SymbolTable symbolTable;
    Expression expr("list(2)", symbolTable);
    Expression expr1("list(2,2)", symbolTable);
    Expression expr2("list(4,2,3)", symbolTable);
    Expression expr3("list(list(5,1,3))", symbolTable);
    Expression expr4("list([1,2,3,4])", symbolTable);
    Expression expr5("list([])", symbolTable);
    Expression expr6("list([1])", symbolTable);

    Literal* result = expr.calculate();
    Literal* result1 = expr1.calculate();
    Literal* result2 = expr2.calculate();

    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr4.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr5.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr6.calculate(), std::invalid_argument);

    REQUIRE(*result == List(2));
    REQUIRE(*result1 == List(2, 2));
    REQUIRE(*result2 == List(4, 2, 3));

    delete result;
    delete result1;
    delete result2;
}

TEST_CASE("Expression concat")
{
    SymbolTable symbolTable;
    Expression expr("concat(2, [3,4])", symbolTable);
    Expression expr1("concat([2,3],[4,5])", symbolTable);
    Expression expr2("concat([1,2], list(3))", symbolTable);
    Expression expr3("concat(concat([1],[2]), [3])", symbolTable);
    Expression expr4("concat(list(5), [1,2,3,4])", symbolTable);
    Expression expr5("concat(list(5), [])", symbolTable);
    Expression expr6("concat(list(5,1,3))", symbolTable);

    Literal* result = expr.calculate();
    Literal* result1 = expr1.calculate();
    Literal* result2 = expr2.calculate();
    Literal* result3 = expr3.calculate();

    REQUIRE_THROWS_AS(expr4.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr5.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr6.calculate(), std::invalid_argument);

    REQUIRE(*result == List(2, 1, 3));
    REQUIRE(*result1 == List(2, 1, 4));
    REQUIRE(*result2 == List(1, 1, 2).concat(List(3)));
    REQUIRE(*result3 == List(1, 1, 3));

    delete result;
    delete result1;
    delete result2;
    delete result3;
}

TEST_CASE("Expression int")
{
    SymbolTable symbolTable;
    Expression expr("int(1)", symbolTable);
    Expression expr1("int(1.1)", symbolTable);
    Expression expr2("int(1,2,3)", symbolTable);
    Expression expr3("int(1,2)", symbolTable);

    Literal* result = expr.calculate();
    Literal* result1 = expr1.calculate();
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(1));
    REQUIRE(*result1 == Double(1));

    delete result;
    delete result1;
}

TEST_CASE("Expression nand")
{
    SymbolTable symbolTable;
    Expression expr("nand(0,0)", symbolTable);
    Expression expr1("nand(0,1)", symbolTable);
    Expression expr2("nand(1,0)", symbolTable);
    Expression expr3("nand(1,1)", symbolTable);
    Expression expr4("nand(1,1,1)", symbolTable);
    Expression expr5("nand(1)", symbolTable);
    Expression expr6("nand(0)", symbolTable);

    Literal* result = expr.calculate();
    Literal* result1 = expr1.calculate();
    Literal* result2 = expr2.calculate();
    Literal* result3 = expr3.calculate();
    REQUIRE_THROWS_AS(expr4.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr5.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr6.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(1));
    REQUIRE(*result1 == Double(1));
    REQUIRE(*result2 == Double(1));
    REQUIRE(*result3 == Double(0));

    delete result;
    delete result1;
    delete result2;
    delete result3;
}

TEST_CASE("Expression if")
{
    SymbolTable symbolTable;
    Expression expr("if(0,1,2)", symbolTable);
    Expression expr1("if(1,1,2)", symbolTable);
    Expression expr2("if(1,0,  )", symbolTable);
    Expression expr3("if(1)", symbolTable);
    Expression expr4("if(1,1,1,3)", symbolTable);
    Expression expr5("if()", symbolTable);
    Expression expr6("if([1], [1,2], [1,2,3])", symbolTable);
    Expression expr7("if(nand(1,1), list(1,1,3), 5)", symbolTable);
    Expression expr8("if(nand(1,0), list(1,1,3), 5)", symbolTable);

    Literal* result = expr.calculate();
    Literal* result1 = expr1.calculate();
    Literal* result6 = expr6.calculate();
    Literal* result7 = expr7.calculate();
    Literal* result8 = expr8.calculate();
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr4.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr5.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(2));
    REQUIRE(*result1 == Double(1));
    REQUIRE(*result6 == List(1, 1, 2));
    REQUIRE(*result7 == Double(5));
    REQUIRE(*result8 == List(1, 1, 3));

    delete result;
    delete result1;
    delete result6;
    delete result7;
    delete result8;
}

/// ------------------- Arithmetic operators ---------------------
TEST_CASE("Expression +")
{
    SymbolTable symbolTable;
    Expression expr("1 + 2", symbolTable);
    Expression expr2("1 + 2 3", symbolTable);
    Expression expr3("1 +", symbolTable);
    Expression expr4("[1] + 2", symbolTable);

    Literal* result = expr.calculate();
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr4.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(3));

    delete result;
}

TEST_CASE("Expression -")
{
    SymbolTable symbolTable;
    Expression expr("1-2", symbolTable);
    Expression expr2("1-2 3", symbolTable);
    Expression expr3("1 -", symbolTable);
    Expression expr4("[1] - 2", symbolTable);

    Literal* result = expr.calculate();
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr4.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(-1));

    delete result;
}

TEST_CASE("Expression *")
{
    SymbolTable symbolTable;
    Expression expr("1 * 2", symbolTable);
    Expression expr2("1 * 2 3", symbolTable);
    Expression expr3("1 * ", symbolTable);
    Expression expr4("[1] * 2", symbolTable);

    Literal* result = expr.calculate();
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr4.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(2));

    delete result;
}

TEST_CASE("Expression /")
{
    SymbolTable symbolTable;
    Expression expr("1 / 2", symbolTable);
    Expression expr2("1 / 2 3", symbolTable);
    Expression expr3("2 / ", symbolTable);
    Expression expr4("[1] / 2", symbolTable);

    Literal* result = expr.calculate();
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr4.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(0.5));

    delete result;
}

TEST_CASE("Expression ^")
{
    SymbolTable symbolTable;
    Expression expr("1 ^ 2", symbolTable);
    Expression expr2("1 ^ 2 3", symbolTable);
    Expression expr3("2 ^ ", symbolTable);
    Expression expr4("[1] ^ 2", symbolTable);

    Literal* result = expr.calculate();
    REQUIRE_THROWS_AS(expr2.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr4.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(1));

    delete result;
}

TEST_CASE("Expression %")
{
    SymbolTable symbolTable;
    Expression expr("3 % 2", symbolTable);
    Expression expr1("4 % 2", symbolTable);
    Expression expr3("4 % 2 5", symbolTable);
    Expression expr4("4 %", symbolTable);
    Expression expr5("4.2 % 2", symbolTable);
    Expression expr6("4 % 2.2", symbolTable);

    Literal* result = expr.calculate();
    Literal* result1 = expr1.calculate();
    REQUIRE_THROWS_AS(expr3.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr4.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr5.calculate(), std::invalid_argument);
    REQUIRE_THROWS_AS(expr6.calculate(), std::invalid_argument);

    REQUIRE(*result == Double(1));
    REQUIRE(*result1 == Double(0));

    delete result;
    delete result1;
}

/// ------------------- Arithmetic expressions ---------------------
TEST_CASE("Arithmetic expressions")
{
    SymbolTable symbolTable;
    Expression expr("+1 + (-2) ^ 2", symbolTable);
    Expression expr1("4 % 2 + 3 * (5 + (3 * 6 ^ (1 + 1)) -5)", symbolTable);
    Expression expr2("4 % 2 - 5 / 3 * 2 + 5", symbolTable);
    Expression expr3("5 - 3 + if(1,2,3)", symbolTable);

    Literal* result = expr.calculate();
    Literal* result1 = expr1.calculate();
    Literal* result2 = expr2.calculate();
    Literal* result3 = expr3.calculate();

    REQUIRE(*result == Double(+1 + std::pow((-2), 2)));
    REQUIRE(*result1 == Double(4 % 2 + 3 * (5 + (3 * std::pow(6, (1 + 1))) -5)));
    REQUIRE(*result2 == Double(4 % 2 - 5.0 / 3 * 2 + 5));
    REQUIRE(*result3 == Double(5 - 3 + 2));

    delete result;
    delete result1;
    delete result2;
    delete result3;
}

/// --------------------- Recursion -----------------------
TEST_CASE("Expression recursion")
{
    SymbolTable symbolTable;
    symbolTable.add_definition("len", std::make_pair(1, "if(#0,len(tail(#0)) + 1,0)"));
    Expression expr("len(list(1,1,25))", symbolTable);

    Literal* result = expr.calculate();

    REQUIRE(*result == Double(25));

    delete result;
}