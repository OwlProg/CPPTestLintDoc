cmake_minimum_required(VERSION 3.17)

project(CPPTestLintDoc)

set(CMAKE_CXX_STANDARD 17)

# Add your components here
set(SOURCE_FILES AutoDoc/RunAutoDoc.cpp AutoDoc/AutoDoc.cpp Utils/ConfigProcessing.cpp Utils/Exceptions.cpp Utils/Tokenizer.cpp Utils/StringTools.cpp config.yml Linter/FileLinter.cpp Linter/Tabs.cpp Linter/LinterRun.cpp Tester/Tester.cpp Tester/math.cpp Tester/math.test.cpp)
set(HEADER_FILES AutoDoc/AutoDoc.h Utils/ConfigProcessing.h Utils/Exceptions.h Utils/Tokenizer.h Utils/StringTools.h Linter/FileLinter.h Linter/Tabs.h Tester/Tester.h)

# If you have any executable files - add new target this way
set(DOCGEN_FILES CPPTestLintDoc/AutoDoc/AutoDoc.h CPPTestLintDoc/Utils/ConfigProcessing.h CPPTestLintDoc/Utils/Exceptions.h CPPTestLintDoc/Utils/Tokenizer.h CPPTestLintDoc/Utils/StringTools.h CPPTestLintDoc/AutoDoc/RunAutoDoc.cpp CPPTestLintDoc/AutoDoc/AutoDoc.cpp CPPTestLintDoc/Utils/ConfigProcessing.cpp CPPTestLintDoc/Utils/Exceptions.cpp CPPTestLintDoc/Utils/Tokenizer.cpp CPPTestLintDoc/Utils/StringTools.cpp CPPTestLintDoc/config.yml PARENT_SCOPE)

set(LINTER_FILES CPPTestLintDoc/Linter/FileLinter.h CPPTestLintDoc/Linter/FileLinter.cpp CPPTestLintDoc/Linter/Tabs.h CPPTestLintDoc/Linter/Tabs.cpp CPPTestLintDoc/Linter/LinterRun.cpp CPPTestLintDoc/Utils/ConfigProcessing.h CPPTestLintDoc/Utils/Exceptions.h CPPTestLintDoc/Utils/Tokenizer.h CPPTestLintDoc/Utils/StringTools.h CPPTestLintDoc/Utils/ConfigProcessing.cpp CPPTestLintDoc/Utils/Exceptions.cpp CPPTestLintDoc/Utils/Tokenizer.cpp CPPTestLintDoc/Utils/StringTools.cpp CPPTestLintDoc/config.yml PARENT_SCOPE)

add_library(CPPTestLintDoc STATIC ${HEADER_FILES} ${SOURCE_FILES})

