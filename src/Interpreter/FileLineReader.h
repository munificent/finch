#pragma once

#include <fstream>
#include <iostream>

#include "Macros.h"
#include "String.h"
#include "ILineReader.h"

namespace Finch
{
    using std::ifstream;
    
    // A line reader that reads from a file.
    class FileLineReader : public ILineReader
    {
    public:
        FileLineReader(String fileName);
        
        virtual void Start();
        virtual bool EndOfLines() const;
        virtual String NextLine();
        
    private:
        ifstream mFile;
    };
}