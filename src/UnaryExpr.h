#pragma once

#include <iostream>
#include <memory>

#include "Macros.h"
#include "Expr.h"
#include "String.h"

namespace Finch
{
    using std::auto_ptr;
    
    // AST node for an unary message send: "obj message"
    class UnaryExpr : public Expr
    {
    public:
        UnaryExpr(auto_ptr<Expr> object, String message)
        :   mObject(object),
            mMessage(message)
        {
            std::cout << "parse: unary " << *mObject << " " << message << std::endl;
        }
        
        virtual void Trace(std::ostream & stream) const;

    private:
        // the object receiving the message
        auto_ptr<Expr> mObject;
        
        // the name of the message
        String mMessage;
    };
}