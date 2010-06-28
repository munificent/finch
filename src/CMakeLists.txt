# Filename : src\CMakeLists.txt
# Author   : Zac Brown <zac@zacbrown.org>
# Date     : 06.27.2010

# include ourselves
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add subdirectories to build
add_subdirectory(Base)
add_subdirectory(Compiler)
add_subdirectory(Interpreter)
add_subdirectory(Syntax)

include_directories(
            ${CMAKE_SOURCE_DIR}/src 
            ${CMAKE_SOURCE_DIR}/src/Base 
			${CMAKE_SOURCE_DIR}/src/Compiler
			${CMAKE_SOURCE_DIR}/src/Interpreter
			${CMAKE_SOURCE_DIR}/src/Interpreter/Objects 
			${CMAKE_SOURCE_DIR}/src/Interpreter/Primitives
			${CMAKE_SOURCE_DIR}/src/Syntax
			${CMAKE_SOURCE_DIR}/src/Syntax/AST)

# executable build for CMAKE main
add_executable(finch
            ArrayPrimitives.cpp
            main.cpp
            Repl.cpp
            ReplLineReader.cpp
            Script.cpp
            ArrayExpr.h
            ArrayObject.h
            ArrayPrimitives.h
            Repl.h
            ReplLineReader.h
            Script.h
            SelfExpr.h
            UndefineExpr.h)

target_link_libraries(finch
            Base_Obj
            Compiler_Obj 
            Interpreter_Obj
            Syntax_Obj
            Primitives_Obj
            Objects_Obj
            AST_Obj)