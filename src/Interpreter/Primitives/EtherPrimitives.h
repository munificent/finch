#pragma once

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    // Flow control
    PRIMITIVE(EtherDo);
    PRIMITIVE(EtherIfThen);
    PRIMITIVE(EtherIfThenElse);
    PRIMITIVE(EtherWhileDo);
    
    // Basic IO
    PRIMITIVE(EtherWrite);
}

