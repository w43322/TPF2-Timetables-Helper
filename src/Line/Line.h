#ifndef LINE_H
#define LINE_H

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

    // attributes
    bool hasTimeTable;

// FUNCTIONS
public:
    Line(int id): lineID(id) {}
    void SetAttribute(Attr attr, bool b);
    void AddStation(const Station& station);
};

#endif
