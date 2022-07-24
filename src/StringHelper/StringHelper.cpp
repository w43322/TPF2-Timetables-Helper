#include "StringHelper.h"

int StringHelper::GetIntFromString(std::string str)
{
    str = str.substr(str.find('=') + 2);
    str = str.substr(0, str.find(','));
    return stoi(str);
}
bool StringHelper::GetBoolFromString(std::string str)
{
    str = str.substr(str.find('=') + 2);
    str = str.substr(0, str.find(','));
    return str == "true";
}
ArrDepTime StringHelper::GetArrDepTimeFromString(const std::string &str)
{
    std::string s1, s2, s3, s4;
    s1 = str.substr(str.find('{') + 2);
    s2 = s1.substr(s1.find(',') + 2);
    s3 = s2.substr(s2.find(',') + 2);
    s4 = s3.substr(s3.find(',') + 2);
    s1 = s1.substr(0, s1.find(','));
    s2 = s2.substr(0, s2.find(','));
    s3 = s3.substr(0, s3.find(','));
    s4 = s4.substr(0, s4.find(','));
    return ArrDepTime(std::stoi(s1), std::stoi(s2), std::stoi(s3), std::stoi(s4));
}
Time StringHelper::GetDebounceTimeFromString(const std::string &str)
{
    std::string s1, s2;
    s1 = str.substr(str.find('{') + 2);
    s2 = s1.substr(s1.find(',') + 2);
    s1 = s1.substr(0, s1.find(','));
    s2 = s2.substr(0, s2.find(','));
    return Time(std::stoi(s1), std::stoi(s2));
}
std::string StringHelper::GetStringFromString(std::string str)
{
    str = str.substr(str.find('"') + 1);
    str = str.substr(0, str.find('"'));
    return str;
}
Station::Type StringHelper::GetConditionTypeFromString(const std::string &str)
{
    std::string type = GetStringFromString(str);
    switch(type[0])
    {
        case 'A':
            return Station::Type_ArrDep;
        case 'd':
            return Station::Type_debounce;
        case 'N': default:
            return Station::Type_None;
    }
}
bool StringHelper::isCharToBeIgnored(char chr)
{
    return chr == ',' || chr == '\0' || chr == '\n' || chr == '\r';
}
std::vector<std::string> StringHelper::GetCellsFromLine(const std::string &text)
{
    std::vector<std::string> cells;
    std::string cell;
    for (size_t i = 0, siz = text.length(); i <= siz; ++i)
    {
        char chr = text[i];
        if (isCharToBeIgnored(chr) && !cell.empty())
        {
            cells.push_back(cell);
            //printf("\"%s\"\n", cell.c_str());
            cell.clear();
            while(i + 1 < siz && isCharToBeIgnored(text[i + 1]))
                ++i;
            continue;
        }
        cell += chr;
    }
    return cells;
}
