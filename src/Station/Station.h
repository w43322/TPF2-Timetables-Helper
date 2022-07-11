#ifndef STATION_H
#define STATION_H

#include <vector>
#include <string>

#include "Timing.h"

class Station
{
    friend class Line;
    friend class StringHelper;
    friend class TimeTable;

// DATA
public:
    enum Attr {Attr_conditionType, Attr_inboundTime, Attr_stationID};
private:
    // attributes
    enum Type {Type_ArrDep, Type_debounce, Type_None} conditionType;
    int inboundTime;
    int stationID;

    // self defined
    std::vector<ArrDepTime> arrdepTimes;
    Time debounceTime;
    std::string name = "Alias";

// FUNCTIONS
public:
    void AppendArrDepTime(const ArrDepTime &adt);
    void SetAttribute(Attr attr, Type type);
    void SetAttribute(Attr attr, int i);
    void SetConditionType(Type t);
    void SetDebounceTime(const Time &dbt);
};

#endif
