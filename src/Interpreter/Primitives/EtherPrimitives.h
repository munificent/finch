#pragma once

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    // Primitive methods for the global Ether object.
    //### bob: this should be moved out of core code into something the
    // standalone provides.
    PRIMITIVE(EtherQuit);
    
    // Flow control
    PRIMITIVE(EtherDo);
    PRIMITIVE(EtherIfThen);
    PRIMITIVE(EtherIfThenElse);
    PRIMITIVE(EtherWhileDo);
    
    // Basic IO
    PRIMITIVE(EtherWrite);
    PRIMITIVE(EtherWriteLine);
}

