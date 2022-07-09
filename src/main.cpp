#include <getopt.h>

#include "LuaIO.h"

int main(int argc, char **argv)
{
    int ch;

    std::string inPath, outPath;

    enum Operation {Help, Offset} operation = Help;

    // Offset Operation
    enum OffsetSelect {None, Arr, Dep, ArrDep} offsetSelect = None;
    int OffsetTime = 0;

    //printf("初始值：optind = %d，opterr = %d\n", optind, opterr);

    while ((ch = getopt(argc, argv, "r:w:os:t:")) != -1)
    {
        //printf("开始解析参数：optind = %d\n", optind);
        switch (ch) 
        {
        case 'r':
            printf("Input File Path: \"%s\"\n", optarg);
            inPath = optarg;
            break;
        case 'w':
            printf("Output File Path: \"%s\"\n", optarg);
            outPath = optarg;
            break;
        case 'o':
            printf("Operation: OFFSET\n");
            operation = Offset;
            break;
        case 's':
            switch (optarg[0])
            {
                case 'a': case 'A':
                    offsetSelect = Arr;
                    break;
                case 'd': case 'D':
                    offsetSelect = Dep;
                    break;
                default:
                    offsetSelect = None;
                    break;
            }
            switch (optarg[1])
            {
                case 'a': case 'A':
                    offsetSelect = (OffsetSelect)(offsetSelect | Arr);
                    break;
                case 'd': case 'D':
                    offsetSelect = (OffsetSelect)(offsetSelect | Dep);
                    break;
                default:
                    break;
            }
            printf("    Offset Select: %s\n", optarg);
            break;
        case 't':
            printf("    Offset Time: %s sec(s)\n", optarg);
            break;
        }
    }

    if (inPath.empty() || outPath.empty())
    {
        printf("Error: Input or output file path not set!\nExiting...\n");
        return 0;
    }

    // initialize LuaIO object
    LuaIO luaio(inPath, outPath);
    luaio.Read();

    switch (operation)
    {
    case Offset:
        if (offsetSelect == None)
        {
            printf("Error: Offset mode not selected!\n    Use \"-s a|d|ad\" to select offset mode, \"-h\" for more info.\nExiting...\n");
        }
        break;
    }

    // ./build/TPF2-Timetables-Helper -r "./input.lua" -w "./output.lua" -o -t -55 -s ad

    /*

    luaio.Write();*/

    return 0;
}

/*
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
