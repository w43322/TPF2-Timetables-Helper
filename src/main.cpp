#include <iostream>
#include <fstream>
#include <map>

class Station
{
// DATA
public:
    enum Attr {Attr_conditionType, Attr_inboundTime, Attr_stationID};
private:
    // attributes
    enum Type {Type_ArrDep, Type_debounce, Type_None} conditionType;
    int inboundTime;
    int stationID;

// FUNCTIONS
public:
    void SetAttribute(Attr attr, Type type)
    {
        switch (attr)
        {
        case Attr_conditionType:
            conditionType = type;
            return;
        default:
            return;
        }
    }
    void SetAttribute(Attr attr, int i)
    {
        switch (attr)
        {
        case Attr_inboundTime:
            inboundTime = i;
            return;
        case Attr_stationID:
            stationID = i;
            return;
        default:
            return;
        }
    }
};

class Line
{
// DATA
public:
    enum Attr {Attr_hasTimeTable};
private:
    // self defined
    int lineID;

    // attributes
    bool hasTimeTable;

// FUNCTIONS
public:
    Line(int id): lineID(id)
    {

    }
    void SetAttribute(Attr attr, bool b)
    {
        switch (attr)
        {
        case Attr_hasTimeTable:
            hasTimeTable = b;
            return;
        }
    }
    static int GetIdFromString(std::string str)
    {
        str = str.substr(str.find('"') + 1);
        str = str.substr(0, str.find('"'));
        return stoi(str);
    }
    static bool GetBoolFromString(std::string str)
    {
        str = str.substr(str.find('=') + 2);
        str = str.substr(0, str.find(','));
        return str == "true";
    }
};

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
        Line newLine(Line::GetIdFromString(text));

        // read "hasTimetable" attribute
        std::getline(srcLuaFile, text); // hasTimetable = true,
        newLine.SetAttribute(Line::Attr_hasTimeTable, Line::GetBoolFromString(text));

        // read stations block
        std::getline(srcLuaFile, text); // stations = {
        // read station
        std::getline(srcLuaFile, text); // {
        while (text != "\t\t\t\t},") // ends reading a STATIONS block
        {
            // construct a STATION
                // TODO

            // read conditions block
            std::getline(srcLuaFile, text); // conditions = {
            std::getline(srcLuaFile, text); // ArrDep | debounce | None | type
            while (text != "\t\t\t\t\t\t},") // ends reading a CONDITION block
            {
                // analyze format
                if (text.back() == '{') // breaking line
                {
                    // get time
                    std::getline(srcLuaFile, text);
                    while (text != "\t\t\t\t\t\t\t},") // ends reading a TIME block
                    {
                        // read time
                            // TODO
                        // get new time
                        std::getline(srcLuaFile, text);
                    }
                }
                // else: one line
                // get new line for analyze
                std::getline(srcLuaFile, text); // ArrDep | debounce | None | type
            }
            // read "inboundTime" attribute
                // TODO
            // read "stationID" attribute
                // TODO
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