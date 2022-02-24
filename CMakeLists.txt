cmake_minimum_required(VERSION 3.17)

project(CPPTestLintDoc)

set(CMAKE_CXX_STANDARD 17)

#add_executable(CPPTestLintDoc main.cpp Utils/Tokenizer.cpp Utils/Tokenizer.h Utils/StringTools.cpp Utils/StringTools.h Utils/ConfigProcessing.cpp Utils/ConfigProcessing.h Utils/Exceptions.cpp Utils/Exceptions.h)

# AutoDoc
#add_subdirectory(AutoDoc AutoDoc/)
add_executable(AutoDoc AutoDoc/RunAutoDoc.cpp AutoDoc/AutoDoc.h AutoDoc/AutoDoc.cpp Utils/ConfigProcessing.h Utils/ConfigProcessing.cpp Utils/Exceptions.h Utils/Exceptions.cpp Utils/Tokenizer.h Utils/Tokenizer.cpp Utils/StringTools.h Utils/StringTools.cpp config.yml)


# Global test
add_executable(main main.cpp AutoDoc/AutoDoc.h AutoDoc/AutoDoc.cpp Utils/ConfigProcessing.h Utils/ConfigProcessing.cpp Utils/Exceptions.h Utils/Exceptions.cpp Utils/Tokenizer.h Utils/Tokenizer.cpp Utils/StringTools.h Utils/StringTools.cpp config.yml)