#include "Station.h"

void Station::AppendArrDepTime(const ArrDepTime& adt)
{
    arrdepTimes.push_back(adt);
}
void Station::SetAttribute(Attr attr, Type type)
{
    switch (attr)
    {
    case Attr_conditionType:
        conditionType = type;
        return;
    default:
        return;
    }
}
void Station::SetAttribute(Attr attr, int i)
{
    switch (attr)
    {
    case Attr_inboundTime:
        inboundTime = i;
        return;
    case Attr_stationID:
        stationID = i;
        return;
    default:
        return;
    }
}
void Station::SetConditionType(Type t)
{
    conditionType = t;
}
