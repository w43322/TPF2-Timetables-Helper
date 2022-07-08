#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <fstream>

#include "Line.h"
#include "StringHelper.h"

class TimeTable
{
private:
    std::vector<Line> lines;
public:
    void AddLine(const Line& line);
    void ReadFromFile(std::ifstream &ifs);
    void OutputToFile(std::ofstream &ofs);
};

#endif
