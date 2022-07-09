#include <sstream>

#include "TimeTable.h"

class LuaIO
{
private:
    std::stringstream before;
    std::stringstream after;
    TimeTable tt;
    std::string inPath;
    std::string outPath;
public:
    LuaIO(const std::string &i,
        const std::string &o): inPath(i), outPath(o) {}
    void Read();
    void Write();
};
