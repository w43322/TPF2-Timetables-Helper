#include <iostream>
#include <fstream>
#include <map>

std::map<int, std::string> stationName;
std::map<int, std::string> lineName = 
{
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

int main(int argc, char **argv)
{
    // open input file
    std::ifstream srcLuaFile("../test_recording.sav.lua");
    
    // check if file is open
    if (!srcLuaFile.is_open())
    {
        std::cerr << "Error : Cannot open input file!\n";
        return EXIT_FAILURE;
    }

    // lua file parser
    std::string line;
    // try to find the start of timetable section
    while (line != "\t[\"timetable_gui.lua\"] = {")
        std::getline(srcLuaFile, line);

    while (line != "\t},")
    {
        std::getline(srcLuaFile, line);
    }

    return 0;
}