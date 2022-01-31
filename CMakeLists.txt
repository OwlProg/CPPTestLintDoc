cmake_minimum_required(VERSION 3.17)

project(CPPTestLintDoc)

set(CMAKE_CXX_STANDARD 14)

add_executable(CPPTestLintDoc Utils/Tokenizer.cpp Utils/Tokenizer.h main.cpp VariableChecker/VariableCheck.cpp VariableChecker/VariableCheck.h)