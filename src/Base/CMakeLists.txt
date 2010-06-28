# Filename : src\Base\CMakeLists.txt
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

add_library(Base_Obj 
            FinchString.cpp 
            StringTable.cpp
            Dictionary.h
            Macros.h
            Queue.h
            Ref.h
            Stack.h
            FinchString.h
            StringTable.h)
    