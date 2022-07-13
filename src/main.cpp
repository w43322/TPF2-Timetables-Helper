#ifdef WIN32
    #include <io.h>
    #include "../lib/getopt.h"
    #define dup(x) _dup(x)
    #define dup2(x, y) _dup2(x, y)
#else
    #include <getopt.h>
    #include <unistd.h>
#endif

#include <cstring>
#include <cctype>

#include "LuaIO.h"

int main(int argc, char **argv)
{
    int ch, so;
    FILE *logfileptr;

    std::string inPath, outPath, aliasPath;

    enum Operation {Help, Offset, Generate, Lookup} operation = Help;

    bool quiet = false;

    std::vector<int> IDs;

    // Offset Operation
    TimeTable::OffsetSelect offsetSelect = TimeTable::Select_None;
    int timeInSec;

    // Generate Operation
    std::ifstream srcCsvFile;
    std::ofstream dstCsvFile;

    // Lookup Operation
    std::ifstream tbtdCsvFile;

    //printf("初始值：optind = %d，opterr = %d\n", optind, opterr);


    for (int i = 0; i < argc; ++i)
        if (strcmp(argv[i], "-q") == 0)
            quiet = true;

    if (quiet)
    {
        so = dup(1);
        logfileptr = freopen("log.txt", "w", stdout);
    }

    while ((ch = getopt(argc, argv, "r:w:os:t:i:gc:v:a:ld:q")) != -1)
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
            timeInSec = 0;
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
                printf("    Offset Select: None\n");
                break;
            case TimeTable::Select_Arr:
                printf("    Offset Select: Arr\n");
                break;
            case TimeTable::Select_Dep:
                printf("    Offset Select: Dep\n");
                break;
            case TimeTable::Select_ArrDep:
                printf("    Offset Select: ArrDep\n");
                break;
            }
            break;
        case 't':
            timeInSec = std::stoi(optarg);
            printf("    Time In Sec: %d sec(s)\n", timeInSec);
            break;
        case 'i':
            printf("    IDs: ");
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
                    IDs.push_back(num);
                    num = 0;
                }
            }
            for (auto &&id : IDs)
                printf("%d, ", id);
            if (IDs.empty())
                printf("   (null)");
            printf("\n");
            break;
        case 'g':
            printf("Operation: GENERATE TBTD\n");
            operation = Generate;
            break;
        case 'c':
            srcCsvFile = std::ifstream(optarg);
            printf("    TBTD Input File Path: \"%s\"\n", optarg);
            if (!srcCsvFile.is_open())
            {
                printf("Error: Failed to open TBTD input file!\nExiting...\n");
                return 0;
            }
            break;
        case 'v':
            dstCsvFile = std::ofstream(optarg);
            printf("    TBTD Output File Path: \"%s\"\n", optarg);
            if (!dstCsvFile.is_open())
            {
                printf("Error: Failed to open or create TBTD output file!\nExiting...\n");
                return 0;
            }
            break;
        case 'a':
            printf("Aliases File Path: \"%s\"\n", optarg);
            aliasPath = optarg;
            break;
        case 'l':
            printf("Operation: LOOKUP\n");
            operation = Lookup;
            timeInSec = -1;
            break;
        case 'd':
            tbtdCsvFile = std::ifstream(optarg);
            printf("    TBTD Data data Path: \"%s\"\n", optarg);
            if (!tbtdCsvFile.is_open())
            {
                printf("Error: Failed to open or create TBTD data file!\nExiting...\n");
                return 0;
            }
            break;
        case 'q':
            quiet = true;
            break;
        }
    }

    LuaIO luaio(inPath, outPath, aliasPath);

    switch (operation)
    {
    case Offset:
        if (inPath.empty() || outPath.empty())
        {
            printf("Error: Input or output file path not set!\nExiting...\n");
            return 0;
        }
        if (aliasPath.empty())
            printf("Warning: Alias file not specified!\n");
        luaio.Read();
        if (offsetSelect == TimeTable::Select_None)
        {
            printf("Error: Offset mode not selected!\n    "
                "Use \"-s a|d|ad\" to select offset mode, \"-h\" for more info.\nExiting...\n");
            break;
        }
        if (timeInSec == 0)
        {
            printf("Error: Offset time not selected or equals to zero!\n    "
                "Use \"-t xx\" to select offset mode, \"-h\" for more info.\nExiting...\n");
            break;
        }
        if (IDs.empty())
            printf("Warning: You didn't specify which lines to offset, defaulting to all lines...\n");
        luaio.tt.Offset(IDs, timeInSec, offsetSelect);
        printf("Writing to specified output file...\n");
        luaio.Write();
        break;
    case Generate:
        if (inPath.empty() || outPath.empty())
        {
            printf("Error: Input or output file path not set!\nExiting...\n");
            return 0;
        }
        if (aliasPath.empty())
            printf("Warning: Alias file not specified!\n");
        luaio.Read();
        luaio.tt.GenerateTBTD(srcCsvFile, dstCsvFile);
        break;
    case Lookup:
        {
            if (IDs.size() != 3)
            {
                printf("Error: In lookup mode, -i takes EXACTLY 3 arguments!\n    "
                    "They are lineID, origID, destID.\nExiting...\n");
                break;
            }
            if (timeInSec == -1)
            {
                printf("Error: Time not selected!"
                    "Exiting...\n");
                break;
            }
            ArrDepTime res = luaio.tt.LookupTBTD(tbtdCsvFile, IDs[0], IDs[1], IDs[2], timeInSec);
            if (quiet)
            {
                fflush(logfileptr);
                dup2(so, 1);
            }
            printf("%d %d\n", res.dep.Seconds(), res.arr.Seconds());
            if (quiet)
            {
                so = dup(1);
                logfileptr = freopen("log.txt", "a", stdout);
            }
        }
        break;
    case Help:
        break;
    }

    // ./build/TPF2-Timetables-Helper -r "./input.lua" -w "./output.lua" -a "./aliases.csv" -o -t 1 -s ad -i 123

    // ./build/TPF2-Timetables-Helper -r "./input.lua" -w "./output.lua" -a "./aliases.csv" -g -c "./input.csv" -v "./output.csv"

    // ./build/TPF2-Timetables-Helper -q -l -d "./output.csv" -i 288194,216714,145798 -t 1500

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
