#include <iostream>

#include "ReplLineReader.h"

namespace Finch
{
    using std::cin;
    using std::cout;
    using std::endl;
    using std::string;
    
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
        
        string line;
        getline(cin, line);
        
        return String(line.c_str());
    }
}
