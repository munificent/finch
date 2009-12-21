#include "FileLineReader.h"

#include <iostream>

namespace Finch
{
    using std::ios;
    
    FileLineReader::FileLineReader(String fileName)
    {
        mFile.open(fileName.c_str(), ios::in);
    }

    bool FileLineReader::EndOfLines() const
    {
        if (!mFile) return true;
                
        return mFile.eof();
    }
    
    String FileLineReader::NextLine()
    {
        ASSERT(mFile, "Cannot call NextLine() on a missing file.");
        
        String line;
        getline(mFile, line);
        
        return line;
    }
}
