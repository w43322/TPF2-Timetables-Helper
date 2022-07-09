#include "TimeTable.h"

void TimeTable::AddLine(const Line& line)
{
    lines.insert(std::make_pair(line.lineID, line));
}
void TimeTable::ReadFromFile(std::ifstream &ifs,
                        std::stringstream &before,
                        std::stringstream &after)
{
    // check if file is open
    if (!ifs.is_open())
    {
        printf("Error: Input file error!\n");
        return;
    }

    // lua file parser
    std::string text;
    // try to find the start of timetable block
    while (true)
    {
        std::getline(ifs, text);
        if (text == "\t\ttimetable = {")
            break;
        before << text << '\n';
    }

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
                case 'd':
                    if (text[20] != '}') // debounce = { 0, 0, },
                        newStation.SetDebounceTime(StringHelper::GetDebounceTimeFromString(text));
                    break;
                case 't': // type = "ArrDep",
                    newStation.SetConditionType(StringHelper::GetConditionTypeFromString(text));
                    break;
                case 'N': default: // type = "None",
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

    // write remainder of file
    while (true)
    {
        std::getline(ifs, text);
        if (text.empty())
            break;
        after << text << '\n';
    }
}
void TimeTable::OutputToFile(std::ofstream &ofs,
                    const std::stringstream &before,
                    const std::stringstream &after)
{
    // check if file is open
    if (!ofs.is_open())
    {
        printf("Error: Output file error!\n");
        return;
    }
    
    ofs << before.str();

    // timetable start
    ofs << "\t\ttimetable = {\n";

    for (auto &&_line : lines)
    {
        auto &&line = _line.second;

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
                // debounce = { 0, 0, },
                ofs << "\t\t\t\t\t\t\tdebounce = { ";
                ofs << std::to_string(station.debounceTime.mm);
                ofs << ", ";
                ofs << std::to_string(station.debounceTime.ss);
                ofs << ", },\n";
                ofs << "\t\t\t\t\t\t\ttype = \"debounce\",\n";
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
    ofs << "\t\t},\n";

    ofs << after.str();
}
void TimeTable::Offset(const std::vector<int> &lineIDs,
                    int seconds,
                    OffsetSelect sel)
{
    if(!lineIDs.empty()) for (auto &&id : lineIDs)
    {
        if (lines.find(id) == lines.end())
        {
            printf("Warning: Line ID %d not found, skipping...\n", id);
            continue;
        }
        auto &&line = lines.at(id);
        for (auto &&station : line.stations)
        {
            for (auto &&tim : station.arrdepTimes)
            {
                if (sel & 0b01) // Arr
                    tim.arr = tim.arr + seconds;
                if (sel & 0b10) // Deo
                    tim.dep = tim.dep + seconds;
            }
        }
    }
    else for (auto &&_line : lines)
    {
        auto &&line = _line.second;
        for (auto &&station : line.stations)
        {
            for (auto &&tim : station.arrdepTimes)
            {
                if (sel & 0b01) // Arr
                    tim.arr = tim.arr + seconds;
                if (sel & 0b10) // Deo
                    tim.dep = tim.dep + seconds;
            }
        }
    }
}
