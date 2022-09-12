#include <sstream>

#include "TimeTable.h"

class LuaIO
{
private:
    std::stringstream before;
    std::stringstream after;
    std::string inPath;
    std::string outPath;
    std::string aliasPath;
public:
    LuaIO(const std::string &i,
        const std::string &o,
        const std::string &a): inPath(i), outPath(o), aliasPath(a) {}
    void Read(const std::vector<int> &IDs);
    void Write(const std::vector<int> &IDs);
    TimeTable tt;
};
