#include "TimeTable.h"

void TimeTable::AddLine(const Line& line)
{
    lines.push_back(line);
}
void TimeTable::ReadFromFile(std::ifstream &ifs)
{
    // lua file parser
    std::string text;
    // try to find the start of timetable block
    while (text != "\t\ttimetable = {")
        std::getline(ifs, text);

    // read timetable block
    // read LINE block
    std::getline(ifs, text); // ["109701"] = {
    while (text != "\t\t},") // ends reading a TIMETABLE (LINES) block
    {
        // construct a LINE
        Line newLine(stoi(StringHelper::GetStringFromString(text)));

        // read "hasTimetable" attribute
        std::getline(ifs, text); // hasTimetable = true,
        newLine.SetAttribute(Line::Attr_hasTimeTable, StringHelper::GetBoolFromString(text));

        // read stations block
        std::getline(ifs, text); // stations = {
        // read station
        std::getline(ifs, text); // {
        while (text != "\t\t\t\t},") // ends reading a STATIONS block
        {
            // construct a STATION
            Station newStation;

            // read conditions block
            std::getline(ifs, text); // conditions = {
            std::getline(ifs, text); // ArrDep | debounce | None | type
            while (text != "\t\t\t\t\t\t},") // ends reading a CONDITION block
            {
                // analyze format
                if (text.back() == '{') // breaking line (ArrDep)
                {
                    // get time
                    std::getline(ifs, text); // { 42, 15, 42, 35, },
                    while (text != "\t\t\t\t\t\t\t},") // ends reading a TIME block
                    {
                        // read
                        newStation.AppendArrDepTime(StringHelper::GetArrDepTimeFromString(text));
                        // get new time
                        std::getline(ifs, text); // { 42, 15, 42, 35, },
                    }
                }
                // else: one liner (Others)
                switch (text[7])
                {
                case 'd': // debounce = { 0, 0, },
                    // TODO
                    break;
                case 't': // type = "ArrDep",
                    newStation.SetConditionType(StringHelper::GetConditionTypeFromString(text));
                    break;
                case 'N': default:
                    break;
                }
                // get new line for analyze
                std::getline(ifs, text); // ArrDep | debounce | None | type
            }
            // read "inboundTime" attribute
            std::getline(ifs, text); // inboundTime = 0,
            newStation.SetAttribute(Station::Attr_inboundTime, StringHelper::GetIntFromString(text));
            // read "stationID" attribute
            std::getline(ifs, text); // stationID = 155237,
            newStation.SetAttribute(Station::Attr_stationID, StringHelper::GetIntFromString(text));
            // add STATION to LINE
            newLine.AddStation(newStation);
            // read next STATION
            std::getline(ifs, text); // },
            std::getline(ifs, text); // {
        }
        // add LINE to TT
        AddLine(newLine);
        // read next LINE
        std::getline(ifs, text); // },
        std::getline(ifs, text); // ["109701"] = {
    }
}
void TimeTable::OutputToFile(std::ofstream &ofs)
{
    // timetable start
    ofs << "\t\ttimetable = {\n";

    for (auto &&line : lines)
    {
        // skip lines without timetable
        if (!line.hasTimeTable)
            continue;

        // line start
        ofs << "\t\t\t[\"" << line.lineID << "\"] = {\n";
        
        // hasTimetable
        ofs << "\t\t\t\thasTimetable = " << (line.hasTimeTable ? "true" : "false") << ",\n";

        // stations start
        ofs << "\t\t\t\tstations = {\n";
        
        for (auto &&station : line.stations)
        {
            // station start
            ofs << "\t\t\t\t\t{\n";

            // conditions start
            ofs << "\t\t\t\t\t\tconditions = {\n";

            switch (station.conditionType)
            {
            case Station::Type_ArrDep:
                ofs << "\t\t\t\t\t\t\tArrDep = {\n";
                for (auto &&tim : station.arrdepTimes)
                {
                    ofs << "\t\t\t\t\t\t\t\t{ ";
                    ofs << std::to_string(tim.arr.mm);
                    ofs << ", ";
                    ofs << std::to_string(tim.arr.ss);
                    ofs << ", ";
                    ofs << std::to_string(tim.dep.mm);
                    ofs << ", ";
                    ofs << std::to_string(tim.dep.ss);
                    ofs << ", },\n";
                }
                ofs << "\t\t\t\t\t\t\t},\n";
                ofs << "\t\t\t\t\t\t\ttype = \"ArrDep\",\n";
                break;
            case Station::Type_debounce:
                // TODO
                ofs << "\t\t\t\t\t\t\ttype = \"ArrDep\",\n";
                break;
            case Station::Type_None:
                ofs << "\t\t\t\t\t\t\tNone = { },\n";
                ofs << "\t\t\t\t\t\t\ttype = \"None\",\n";
                break;
            }

            // conditions end
            ofs << "\t\t\t\t\t\t},\n";
            
            // inboundTime
            ofs << "\t\t\t\t\t\tinboundTime = " << station.inboundTime << ",\n";

            // stationID
            ofs << "\t\t\t\t\t\tstationID = " << station.stationID << ",\n";

            // station end
            ofs << "\t\t\t\t\t},\n";
        }

        // stations end
        ofs << "\t\t\t\t},\n";

        // line end
        ofs << "\t\t\t},\n";
    }

    // timetable end
    ofs << "\t\t},";
}
