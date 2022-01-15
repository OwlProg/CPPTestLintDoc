cmake_minimum_required(VERSION 3.17)

project(CPPTestLintDoc)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPPTestLintDoc Utils/Tokenizer.cpp Utils/Tokenizer.h AutoDoc/AutoDoc.cpp AutoDoc/AutoDoc.h)