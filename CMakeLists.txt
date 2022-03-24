cmake_minimum_required(VERSION 3.17)

project(CPPTestLintDoc)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPPTestLintDoc main.cpp AutoDoc/AutoDoc.cpp AutoDoc/AutoDoc.h config.yml Utils/Tokenizer.cpp Utils/Tokenizer.h Utils/StringTools.cpp Utils/StringTools.h Utils/ConfigProcessing.cpp Utils/ConfigProcessing.h Utils/Exceptions.cpp Utils/Exceptions.h Linter/FileLinter.h Linter/FileLinter.cpp Linter/Tabs.cpp Linter/Tabs.h Linter/LinterRun.cpp)

# AutoDoc
add_executable(AutoDoc AutoDoc/RunAutoDoc.cpp AutoDoc/AutoDoc.h AutoDoc/AutoDoc.cpp Utils/ConfigProcessing.h Utils/ConfigProcessing.cpp Utils/Exceptions.h Utils/Exceptions.cpp Utils/Tokenizer.h Utils/Tokenizer.cpp Utils/StringTools.h Utils/StringTools.cpp config.yml)


# Global test
#add_executable(main main.cpp AutoDoc/AutoDoc.h AutoDoc/AutoDoc.cpp Utils/ConfigProcessing.h Utils/ConfigProcessing.cpp Utils/Exceptions.h Utils/Exceptions.cpp Utils/Tokenizer.h Utils/Tokenizer.cpp Utils/StringTools.h Utils/StringTools.cpp config.yml)

# Final CMakeLists.txt will look like this:
#cmake_minimum_required(VERSION 3.17)
#
#project(CPPTestLintDoc)
#
#set(CMAKE_CXX_STANDARD 17)
#
# Add your components here
#set(SOURCE_FILES AutoDoc/RunAutoDoc.cpp AutoDoc/AutoDoc.cpp Utils/ConfigProcessing.cpp Utils/Exceptions.cpp Utils/Tokenizer.cpp Utils/StringTools.cpp config.yml)
#set(HEADER_FILES AutoDoc/AutoDoc.h Utils/ConfigProcessing.h Utils/Exceptions.h Utils/Tokenizer.h Utils/StringTools.h)
#
# If you have any executable files - add new target this way
#set(DOCGEN_FILES src/AutoDoc/AutoDoc.h src/Utils/ConfigProcessing.h src/Utils/Exceptions.h src/Utils/Tokenizer.h src/Utils/StringTools.h src/AutoDoc/RunAutoDoc.cpp src/AutoDoc/AutoDoc.cpp src/Utils/ConfigProcessing.cpp src/Utils/Exceptions.cpp src/Utils/Tokenizer.cpp src/Utils/StringTools.cpp src/config.yml PARENT_SCOPE)