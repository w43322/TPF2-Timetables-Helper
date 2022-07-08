#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "Line.h"
#include "Station.h"
#include "StringHelper.h"
#include "Timing.h"

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
    std::string text;
    // try to find the start of timetable block
    while (text != "\t\ttimetable = {")
        std::getline(srcLuaFile, text);

    // read timetable block
    // read LINE block
    std::getline(srcLuaFile, text); // ["109701"] = {
    while (text != "\t\t},") // ends reading a TIMETABLE (LINES) block
    {
        // construct a LINE
        Line newLine(stoi(StringHelper::GetStringFromString(text)));

        // read "hasTimetable" attribute
        std::getline(srcLuaFile, text); // hasTimetable = true,
        newLine.SetAttribute(Line::Attr_hasTimeTable, StringHelper::GetBoolFromString(text));

        // read stations block
        std::getline(srcLuaFile, text); // stations = {
        // read station
        std::getline(srcLuaFile, text); // {
        while (text != "\t\t\t\t},") // ends reading a STATIONS block
        {
            // construct a STATION
            Station newStation;

            // read conditions block
            std::getline(srcLuaFile, text); // conditions = {
            std::getline(srcLuaFile, text); // ArrDep | debounce | None | type
            while (text != "\t\t\t\t\t\t},") // ends reading a CONDITION block
            {
                // analyze format
                if (text.back() == '{') // breaking line (ArrDep)
                {
                    // get time
                    std::getline(srcLuaFile, text); // { 42, 15, 42, 35, },
                    while (text != "\t\t\t\t\t\t\t},") // ends reading a TIME block
                    {
                        // read time
                        ArrDepTime time = StringHelper::GetArrDepTimeFromString(text);
                        // get new time
                        std::getline(srcLuaFile, text); // { 42, 15, 42, 35, },
                    }
                }
                // else: one liner (Others)
                switch (text[7])
                {
                case 'd': // debounce = { 0, 0, },
                    break;
                case 't': // type = "ArrDep",
                    newStation.SetConditionType(StringHelper::GetConditionTypeFromString(text));
                    break;
                case 'N': default:
                    break;
                }
                // get new line for analyze
                std::getline(srcLuaFile, text); // ArrDep | debounce | None | type
            }
            // read "inboundTime" attribute
            std::getline(srcLuaFile, text); // inboundTime = 0,
            newStation.SetAttribute(Station::Attr_inboundTime, StringHelper::GetIntFromString(text));
            // read "stationID" attribute
            std::getline(srcLuaFile, text); // stationID = 155237,
            newStation.SetAttribute(Station::Attr_stationID, StringHelper::GetIntFromString(text));
            // read next STATION
            std::getline(srcLuaFile, text); // },
            std::getline(srcLuaFile, text); // {
        }
        // read next LINE
        std::getline(srcLuaFile, text); // },
        std::getline(srcLuaFile, text); // ["109701"] = {
    }

    return 0;
}
