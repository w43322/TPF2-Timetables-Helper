#ifndef STATION_H
#define STATION_H

#include <vector>

#include "Timing.h"

class Station
{
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

// FUNCTIONS
public:
    void AppendArrDepTime(const ArrDepTime& adt);
    void SetAttribute(Attr attr, Type type);
    void SetAttribute(Attr attr, int i);
    void SetConditionType(Type t);
};

#endif
