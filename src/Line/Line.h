#ifndef LINE_H
#define LINE_H

#include <string>

#include "Station.h"
#include "Timing.h"

class Line
{
    friend class TimeTable;

// DATA
public:
    enum Attr {Attr_hasTimeTable};
private:
    // self defined
    int lineID;
    std::vector<Station> stations;
    std::string name;

    // attributes
    bool hasTimeTable;

// FUNCTIONS
public:
    Line(int id, std::string n = "Name"): lineID(id), name(n) {}
    void SetAttribute(Attr attr, bool b);
    void AddStation(const Station& station);
    int SortStationTimes();
    void GetIndex(int staID, int &staIDX, int &timeIDX, const ArrDepTime &adt);
};

#endif
