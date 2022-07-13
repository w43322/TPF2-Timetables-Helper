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

void TimeTable::ReadAlias(std::ifstream &ifs)
{
    // check if file is open
    if (!ifs.is_open())
    {
        printf("Error: Alias file error!\n");
        return;
    }
    
    // keep reading file until end
    while (true)
    {
        std::string text;
        std::vector<std::string> cells;
        std::getline(ifs, text);
        
        if (text.empty())
            break;
        
        cells = StringHelper::GetCellsFromLine(text);
        
        int id = stoi(cells[0]);

        if (lines.find(id) == lines.end())
            printf("Warning: Line ID %d not found, "
                    "its aliases will not be added...\n", id);
        else
        {
            lines.at(id).name = cells[1];
            for (size_t i = 1, siz = cells.size(); i < siz; ++i)
            {
                std::transform(cells[i].begin(), cells[i].end(), cells[i].begin(), ::tolower);
                aliases.insert(std::make_pair(cells[i], id));
            }
        }
    }

}

void TimeTable::GenerateTBTD(std::ifstream &ifs, std::ofstream &ofs)
{
    std::string text;
    std::vector<std::string> cells;

    // get a line
    std::getline(ifs, text);
    cells = StringHelper::GetCellsFromLine(text);
    while (cells.size() == 1)
    {
        std::vector<int> tripStations/*stores the indexes of stations*/, indexes;
        std::vector<std::vector<int>> trips; // stores the index in Stations::times, trips[trip][staIDX] = timIDX

        if (cells[0] == "")
            break;

        int id;
        if (isdigit(cells[0][0]))
            id = std::stoi(cells[0]);
        else
        {
            std::transform(cells[0].begin(), cells[0].end(), cells[0].begin(), ::tolower);
            if (aliases.find(cells[0]) == aliases.end())
            {
                printf("Error: \"%s\" is not a valid alias!\n"
                    "Exiting...\n", cells[0].c_str());
                return;
            }
            id = aliases.at(cells[0]);
        }
        //printf("\"%s\"\n", cells[0].c_str());

        if (lines.find(id) == lines.end())
        {
            printf("Error: Line ID %d not found while generating it's TBTD!\n"
                "Exiting...\n", id);
            return;
        }

        auto &&line = lines.at(id);

        int timSiz = line.SortStationTimes();
        if (timSiz == 0)
            return;
        
        // get stations
        std::getline(ifs, text);
        cells = StringHelper::GetCellsFromLine(text);
        
        // find refrence trip
        while (cells.size() == 5)
        {
            // get index
            int stationID = std::stoi(cells[0]);
            int staIDX, timeIDX;
            /*printf("\"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",
                cells[0].c_str(),
                cells[1].c_str(),
                cells[2].c_str(),
                cells[3].c_str(),
                cells[4].c_str());*/
            line.GetIndex(stationID, staIDX, timeIDX,
                    ArrDepTime(
                        std::stoi(cells[1]),
                        std::stoi(cells[2]),
                        std::stoi(cells[3]),
                        std::stoi(cells[4])));

            if (staIDX == -1)
            {
                printf("Error: Station %d not found!\n"
                    "Exiting...\n", stationID);
                return;
            }
            tripStations.push_back(staIDX);


            if (timeIDX == -1)
            {
                printf("Error: ArrDepTime not found in station %d!\n"
                    "Exiting...\n", stationID);
                return;
            }
            indexes.push_back(timeIDX);

            // get next station
            std::getline(ifs, text);
            cells = StringHelper::GetCellsFromLine(text);
        }
        trips.push_back(indexes);
        
        // find other trips
        while (true)
        {
            for (auto &&idx : indexes)
            {
                ++idx;
                if (idx == timSiz)
                    idx = 0;
            }
            if (indexes == trips[0])
                break;
            else
                trips.push_back(indexes);
        }

        ofs << id << '\n';
        for (size_t i = 0, siz = tripStations.size(); i < siz; ++i)
        {
            auto &&station = line.stations[tripStations[i]];
            ofs << station.stationID << ",\"" << station.name << "\",";
            //ofs << tripStations[i] << ':';
            for (auto &&trip : trips) // num. elem in trips: stations
            {
                auto &&timeIDX = trip[i];
                //ofs << timeIDX << ',';
                ofs << station.arrdepTimes[timeIDX].arr.Seconds() << ',';
                ofs << station.arrdepTimes[timeIDX].dep.Seconds() << ',';
            }
            ofs << '\n';
        }
    }
}
ArrDepTime TimeTable::LookupTBTD(std::ifstream &ifs, int lineID, int origID, int destID, int startTime)
{
    std::string text;
    std::vector<std::string> cells;
    std::vector<std::vector<std::string>> table;

    // get line with certain id
    while (text != std::to_string(lineID) && (aliases.find(text) == aliases.end() || aliases.at(text) != lineID))
    {
        std::getline(ifs, text);
        cells = StringHelper::GetCellsFromLine(text);
    }

    // get its stations
    struct STATION
    {
        int id;
        ArrDepTime tim;
        STATION(int i, int t1, int t2) : id(i), tim(t1, t2) {}
    };
    
    std::vector<std::vector<STATION>> loops;

    std::getline(ifs, text);
    cells = StringHelper::GetCellsFromLine(text);
    while (cells.size() >= 4)
    {
        // add to table
        table.push_back(cells);
        // get next station
        std::getline(ifs, text);
        cells = StringHelper::GetCellsFromLine(text);
    }

    // read STATIONs from table
    uint64_t mask = UINT64_MAX;
    int col = 2;
    size_t colCnt = table.front().size();
    for (int i = colCnt / 2 - 1; i < UINT64_WIDTH; ++i)
    {
        mask &= ~(1ull << i);
        col += 2;
    }
    col = 2;
    while (mask)
    {
        std::vector<STATION> STATIONs;
        do
        {
            mask &= ~(1 << (col / 2 - 1));
            int id, t1, t2;
            bool skippedFirst = false;
            for (auto &&row : table)
            {
                if (skippedFirst == false)
                {
                    skippedFirst = true;
                    continue;
                }
                id = stoi(row[0]);
                t1 = stoi(row[col]);
                t2 = stoi(row[col + 1]);
                STATIONs.push_back(STATION(id, t1, t2));
            }

            col = 2;
            while(col < colCnt && t1 != stoi(table.front()[col])
                && t2 != stoi(table.front()[col + 1]))
                col += 2;
        } while (col != 2);

        // copy STATIONs and append to itself's end to prepare for lookup
        STATIONs.insert(STATIONs.end(), STATIONs.begin(), STATIONs.end());

        // add STATIONs to loops
        loops.push_back(STATIONs);
    }

    int bestTime = INT_MAX;
    ArrDepTime res(0, 0);
    for (auto &&STATIONs : loops)
    {
        // Debug : Print all stations
        //for (auto &&STATION : STATIONs)
        //    printf("%d, %d, %d\n", STATION.id, STATION.tim.arr.Seconds(), STATION.tim.dep.Seconds());

        for (size_t i = 0, siz = STATIONs.size(); i + 1 < siz; ++i)
        {
            auto &&STATION_1 = STATIONs[i];
            if (STATION_1.id != origID)
                continue;
            for (size_t j = i + 1; j < siz; ++j)
            {
                auto &&STATION_2 = STATIONs[j];
                if (STATION_2.id != destID)
                    continue;
                int depTime = STATION_1.tim.dep.Seconds();
                int arrTime = STATION_2.tim.arr.Seconds();
                int waitTime = (depTime + 3600 - startTime) % 3600;
                int tripTime = (arrTime + 3600 - depTime) % 3600;
                if (waitTime + tripTime < bestTime)
                {
                    res = ArrDepTime(arrTime, depTime);
                    bestTime = waitTime + tripTime;
                }
                break;
            }
        }
    }
    
    return res;
}

