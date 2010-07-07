#include "FileLineReader.h"

#include <iostream>
#include <string>

namespace Finch
{
    using std::ios;
    using std::string;
    
    FileLineReader::FileLineReader(String fileName)
    {
        mFile.open(fileName.CString(), ios::in);
    }
    
    bool FileLineReader::IsInfinite() const
    {
        return false;
    }
    
    bool FileLineReader::EndOfLines() const
    {
        if (!mFile) return true;
                
        return mFile.eof();
    }
    
    String FileLineReader::NextLine()
    {
        ASSERT(mFile, "Cannot call NextLine() on a missing file.");
        
        string line;
        getline(mFile, line);
        
        return String(line.c_str());
    }
}

