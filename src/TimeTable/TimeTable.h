#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <fstream>
#include <sstream>

#include "Line.h"
#include "StringHelper.h"

class TimeTable
{
private:
    std::vector<Line> lines;
public:
    void AddLine(const Line& line);
    void ReadFromFile(std::ifstream &ifs,
                        std::stringstream &before,
                        std::stringstream &after);
    void OutputToFile(std::ofstream &ofs,
                    const std::stringstream &before,
                    const std::stringstream &after);
};

#endif
