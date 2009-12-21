#include <iostream>

#include "ReplLineReader.h"

namespace Finch
{
    using std::cin;
    using std::cout;
    using std::endl;
    
    bool ReplLineReader::EndOfLines() const
    {
        // we can always ask the user for more
        return false;
    }
    
    String ReplLineReader::NextLine()
    {
        if (mFirstLine)
        {
            cout << ">> ";
            mFirstLine = false;
        }
        else
        {
            cout << ".. ";
        }
        
        String line;
        getline(cin, line);
        
        return line;
    }
}
