cmake_minimum_required(VERSION 3.17)

project(CPPTestLintDoc)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPPTestLintDoc main.cpp src/AutoDoc/AutoDoc.cpp src/AutoDoc/AutoDoc.h src/Utils/Utils.cpp src/Utils/Utils.h config.yml src/AutoDoc/RunAutoDoc.cpp)


# AutoDoc DEBUG
#add_executable(test main.cpp)