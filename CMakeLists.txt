cmake_minimum_required(VERSION 3.17)

project(CPPTestLintDoc)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPPTestLintDoc main.cpp AutoDoc/AutoDoc.cpp AutoDoc/AutoDoc.h Utils/Utils.cpp Utils/Utils.h config.yml AutoDoc/RunAutoDoc.cpp)


# AutoDoc DEBUG
#add_executable(test main.cpp)