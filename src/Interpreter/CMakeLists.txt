# Filename : src\Interpreter\CMakeLists.txt
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

# Add Interpreter subdirectories to build
add_subdirectory(Objects)
add_subdirectory(Primitives)

add_library(Interpreter_Obj
            Environment.cpp
            FileLineReader.cpp
            Interpreter.cpp
            Scope.cpp
            Environment.h
            FileLineReader.h
            Interpreter.h
            Scope.h)