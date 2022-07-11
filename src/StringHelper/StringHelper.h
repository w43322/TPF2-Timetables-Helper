#ifndef STRINGHELPER_H
#define STRINGHELPER_H

#include <string>

#include "Timing.h"
#include "Station.h"

class StringHelper
{
public:
    static int GetIntFromString(std::string str);
    static bool GetBoolFromString(std::string str);
    static ArrDepTime GetArrDepTimeFromString(const std::string &str);
    static Time GetDebounceTimeFromString(const std::string &str);
    static std::string GetStringFromString(std::string str);
    static Station::Type GetConditionTypeFromString(const std::string &str);
    static std::vector<std::string> GetCellsFromLine(const std::string &str);
};

#endif
