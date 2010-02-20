#include "BlockObject.h"
#include "CodeBlock.h"
#include "Compiler.h"

namespace Finch
{
    BlockObject::~BlockObject()
    {
        if (mCode != NULL)
        {
            delete mCode;
        }
    }
    
    const CodeBlock & BlockObject::GetCode(Environment & environment) const
    {
        // compile the block the first time it's accessed
        if (mCode == NULL)
        {
            //### bob: arbitrary size :(
            mCode = new CodeBlock(256);
            Compiler::Compile(environment, *mBody, *mCode);
        }
        
        return *mCode;
    }
}