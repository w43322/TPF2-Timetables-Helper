#include "LuaIO.h"

void LuaIO::Read(const std::vector<int> &IDs)
{
    // open input file
    std::ifstream srcLuaFile(inPath);

    // read
    tt.ReadFromFile(srcLuaFile, IDs, before, after);
}

void LuaIO::Write(const std::vector<int> &IDs)
{
    // create output file
    std::ofstream dstLuaFile(outPath);

    // write
    tt.OutputToFile(dstLuaFile, IDs, before, after);
}
