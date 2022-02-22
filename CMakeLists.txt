cmake_minimum_required(VERSION 3.17)

project(CPPTestLintDoc)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPPTestLintDoc main.cpp AutoDoc/AutoDoc.cpp AutoDoc/AutoDoc.h config.yml AutoDoc/RunAutoDoc.cpp Utils/Tokenizer.cpp Utils/Tokenizer.h Utils/StringTools.cpp Utils/StringTools.h Utils/ConfigProcessing.cpp Utils/ConfigProcessing.h Utils/Exceptions.cpp Utils/Exceptions.h)


# AutoDoc
#add_executable(docgen AutoDoc/RunAutoDoc.cpp)