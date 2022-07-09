#include "LuaIO.h"

void LuaIO::Read()
{
    // open input file
    std::ifstream srcLuaFile(inPath);

    // read
    tt.ReadFromFile(srcLuaFile, before, after);
}

void LuaIO::Write()
{
    // create output file
    std::ofstream dstLuaFile(outPath);

    // write
    tt.OutputToFile(dstLuaFile, before, after);
}
