# Filename : src\Syntax\CMakeLists.txt
# Author   : Zac Brown <zac@zacbrown.org>
# Date     : 06.27.2010

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
            ${CMAKE_SOURCE_DIR}/src 
            ${CMAKE_SOURCE_DIR}/src/Base 
            ${CMAKE_SOURCE_DIR}/src/Compiler
            ${CMAKE_SOURCE_DIR}/src/Interpreter
            ${CMAKE_SOURCE_DIR}/src/Interpreter/Objects 
            ${CMAKE_SOURCE_DIR}/src/Interpreter/Primitives
            ${CMAKE_SOURCE_DIR}/src/Syntax
            ${CMAKE_SOURCE_DIR}/src/Syntax/AST)

add_subdirectory(AST)

add_library(Syntax_Obj
            FinchParser.cpp
            Lexer.cpp 
            LineNormalizer.cpp
            Parser.cpp
            Token.cpp
            FinchParser.h
            ILineReader.h
            ITokenSource.h
            Lexer.h
            LineNormalizer.h
            Parser.h
            Token.h)