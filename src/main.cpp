#include <iostream>

#include "TimeTable.h"

#define DEBUG_OUTPUT

class DebugOutput
{
public:
    template <typename T>
    void operator << (const T &str)
    {
    #ifdef DEBUG_OUTPUT
        std::cout << str << '\n';
    #endif
    }
} dout;

/*
std::map<int, std::string> stationName;
std::map<int, std::string> lineName = {
    std::pair<int, std::string>(146978, "GML"),
    std::pair<int, std::string>(83246, "PML"),
    std::pair<int, std::string>(155283, "ECML"),
    std::pair<int, std::string>(61146, "CAL"),
    std::pair<int, std::string>(322592, "Oil"),
    std::pair<int, std::string>(164887, "Subway"),
    std::pair<int, std::string>(132730, "An-Jin"),
    std::pair<int, std::string>(134950, "Freight 1"),
    std::pair<int, std::string>(345197, "Freight 2"),
};
*/

int main(int argc, char **argv)
{
    // construct TT object
    TimeTable tt;

    // open input file
    std::ifstream srcLuaFile("../input.lua");
    
    // check if file is open
    if (!srcLuaFile.is_open())
    {
        std::cerr << "Error : Cannot open input file!\n";
        return EXIT_FAILURE;
    }

    // read input file
    tt.ReadFromFile(srcLuaFile);

    // create output file
    std::ofstream dstLuaFile("../output.lua");

    // output timetable data to file
    tt.OutputToFile(dstLuaFile);

    return 0;
}
