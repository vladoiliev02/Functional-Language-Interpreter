cmake_minimum_required(VERSION 3.21)
project(Project)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        Expression
        Literal
        FunctionParser
        FunctionParser/ExitException
        SymbolTable
        StackFrame
        Interpreter
        ..
)

set(SOURCES
        Expression/Expression.cpp
        Literal/Literal.cpp
        FunctionParser/FunctionParser.cpp
        SymbolTable/SymbolTable.cpp
        StackFrame/StackFrame.cpp
        Interpreter/Interpreter.cpp
        FunctionParser/ExitException/exit_exception.cpp)

add_executable(Project
        main.cpp
        ${SOURCES})

add_executable(LiteralTest
        test/Literal/LiteralTest.cpp
        ${SOURCES})

add_executable(ExpressionTest
        test/Expression/ExpressionTest.cpp
        ${SOURCES})


