#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <fstream>
#include <sstream>
#include <map>

#include "Line.h"
#include "StringHelper.h"

class TimeTable
{
private:
    std::map<int, Line> lines;
public:
    enum OffsetSelect {Select_None, Select_Arr, Select_Dep, Select_ArrDep};
    void AddLine(const Line& line);
    void ReadFromFile(std::ifstream &ifs,
                        std::stringstream &before,
                        std::stringstream &after);
    void OutputToFile(std::ofstream &ofs,
                    const std::stringstream &before,
                    const std::stringstream &after);
    void Offset(const std::vector<int> &lineIDs,
                int seconds,
                OffsetSelect sel);
};

#endif
