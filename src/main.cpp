#include <getopt.h>
#include <cstring>
#include <cctype>

#include "LuaIO.h"

int main(int argc, char **argv)
{
    int ch;

    std::string inPath, outPath;

    enum Operation {Help, Offset, Generate} operation = Help;

    // Offset Operation
    TimeTable::OffsetSelect offsetSelect = TimeTable::Select_None;
    int offsetTime = 0;
    std::vector<int> offsetIDs;

    // Generate Operation
    std::ifstream srcCsvFile;
    std::ofstream dstCsvFile;

    //printf("初始值：optind = %d，opterr = %d\n", optind, opterr);

    while ((ch = getopt(argc, argv, "r:w:os:t:i:gc:v:")) != -1)
    {
        //printf("开始解析参数：optind = %d\n", optind);
        int num = 0;
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
                offsetSelect = TimeTable::Select_Arr;
                break;
            case 'd': case 'D':
                offsetSelect = TimeTable::Select_Dep;
                break;
            default:
                offsetSelect = TimeTable::Select_None;
                break;
            }
            switch (optarg[1])
            {
            case 'a': case 'A':
                offsetSelect = (TimeTable::OffsetSelect)(offsetSelect | TimeTable::Select_Arr);
                break;
            case 'd': case 'D':
                offsetSelect = (TimeTable::OffsetSelect)(offsetSelect | TimeTable::Select_Dep);
                break;
            default:
                break;
            }
            switch (offsetSelect)
            {
            case TimeTable::Select_None:
                printf("    Offset Select: None\n", optarg);
                break;
            case TimeTable::Select_Arr:
                printf("    Offset Select: Arr\n", optarg);
                break;
            case TimeTable::Select_Dep:
                printf("    Offset Select: Dep\n", optarg);
                break;
            case TimeTable::Select_ArrDep:
                printf("    Offset Select: ArrDep\n", optarg);
                break;
            }
            break;
        case 't':
            offsetTime = std::stoi(optarg);
            printf("    Offset Time: %d sec(s)\n", offsetTime);
            break;
        case 'i':
            printf("    Offset Line IDs: ");
            for (size_t i = 0, siz = strlen(optarg); i <= siz; ++i)
            {
                char chr = optarg[i];
                if (isdigit(chr))
                {
                    num *= 10;
                    num += chr - '0';
                }
                else
                {
                    offsetIDs.push_back(num);
                    num = 0;
                }
            }
            for (auto &&id : offsetIDs)
                printf("%d, ", id);
            if (offsetIDs.empty())
                printf("   (null)");
            printf("\n");
            break;
        case 'g':
            printf("Operation: GENERATE TBTD\n");
            operation = Generate;
            break;
        case 'c':
            srcCsvFile = std::ifstream(optarg);
            printf("    TBTD Input File Path: \"%s\n\"", optarg);
            if (!srcCsvFile.is_open())
            {
                printf("Error: Failed to open TBTD input file!\nExiting...\n");
                return 0;
            }
            break;
        case 'v':
            dstCsvFile = std::ofstream(optarg);
            printf("    TBTD Output File Path: \"%s\n\"", optarg);
            if (!dstCsvFile.is_open())
            {
                printf("Error: Failed to open or create TBTD output file!\nExiting...\n");
                return 0;
            }
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
        if (offsetSelect == TimeTable::Select_None)
        {
            printf("Error: Offset mode not selected!\n    "
                "Use \"-s a|d|ad\" to select offset mode, \"-h\" for more info.\nExiting...\n");
            break;
        }
        if (offsetTime == 0)
        {
            printf("Error: Offset time not selected or equals to zero!\n    "
                "Use \"-t xx\" to select offset mode, \"-h\" for more info.\nExiting...\n");
            break;
        }
        if (offsetIDs.empty())
            printf("Warning: You didn't specify which lines to offset, defaulting to all lines...\n");
        luaio.tt.Offset(offsetIDs, offsetTime, offsetSelect);
        printf("Writing to specified output file...\n");
        luaio.Write();
        break;
    case Generate:

        break;
    }

    // ./build/TPF2-Timetables-Helper -r "./test_recording.sav.lua" -w "./output.lua" -o -t 1 -s ad -i 123

    // ./build/TPF2-Timetables-Helper -r "./test_recording.sav.lua" -w "./output.lua" -g -c "./input.csv" -v "./output.csv"

    printf("Program finished.\n");

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
