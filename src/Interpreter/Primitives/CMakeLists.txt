# Filename : src\Interpreter\Primitives\CMakeLists.txt
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

add_library(Primitives_Obj
            BlockPrimitives.cpp
            BooleanPrimitives.cpp
			EtherPrimitives.cpp
			NumberPrimitives.cpp
			ObjectPrimitives.cpp 
			StringPrimitives.cpp
			BlockPrimitives.h
			BooleanPrimitives.h
			EtherPrimitives.h
			NumberPrimitives.h
			ObjectPrimitives.h
			StringPrimitives.h)
