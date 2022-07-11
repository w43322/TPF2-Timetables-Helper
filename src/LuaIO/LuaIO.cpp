#include "LuaIO.h"

void LuaIO::Read()
{
    // open input file
    std::ifstream srcLuaFile(inPath);

    // read
    tt.ReadFromFile(srcLuaFile, before, after);

    if (!aliasPath.empty())
    {
        std::ifstream aliasFile(aliasPath);
        tt.ReadAlias(aliasFile);
    }
}

void LuaIO::Write()
{
    // create output file
    std::ofstream dstLuaFile(outPath);

    // write
    tt.OutputToFile(dstLuaFile, before, after);
}
