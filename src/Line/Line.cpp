#include "Line.h"

void Line::SetAttribute(Attr attr, bool b)
{
    switch (attr)
    {
    case Attr_hasTimeTable:
        hasTimeTable = b;
        return;
    }
}
void Line::AddStation(const Station& station)
{
    stations.push_back(station);
}
int Line::SortStationTimes()
{
    int siz = 0;
    for (auto &&station : stations)
    {
        if (station.conditionType != Station::Type_ArrDep)
            continue;
        if (siz == 0)
            siz = station.arrdepTimes.size();
        if (siz && siz != station.arrdepTimes.size())
        {
            printf("Error: Stations do not have the equal number of arr/dep entries!\nExiting!\n");
            return false;
        }
        std::sort(station.arrdepTimes.begin(), station.arrdepTimes.end());
    }
    return siz;
}
void Line::GetIndex(int staID, int &staIDX, int &timeIDX, const ArrDepTime &adt)
{
    for (size_t i = 0, sizi = stations.size(); i < sizi; ++i)
    {
        auto &&station = stations[i];
        if (station.stationID == staID)
        {
            staIDX = i;
            int j = 0;
            for (size_t j = 0, sizj = station.arrdepTimes.size(); j < sizj; ++j)
            {
                auto &&tim = station.arrdepTimes[j];
                if (tim == adt)
                {
                    timeIDX = j;
                    return;
                }
            }
        }
    }
    timeIDX = -1;
}

