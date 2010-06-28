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

add_library(AST_Obj
            Expr.cpp
            BlockExpr.h
            DefExpr.h
            Expr.h
            IExprVisitor.h
            KeywordExpr.h
            NameExpr.h
            NumberExpr.h
            OperatorExpr.h
            SequenceExpr.h
            SetExpr.h
            StringExpr.h
            UnaryExpr.h)