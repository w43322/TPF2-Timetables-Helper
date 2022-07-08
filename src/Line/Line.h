#ifndef LINE_H
#define LINE_H

#include "Timing.h"

class Line
{
// DATA
public:
    enum Attr {Attr_hasTimeTable};
private:
    // self defined
    int lineID;

    // attributes
    bool hasTimeTable;

// FUNCTIONS
public:
    Line(int id): lineID(id) {}
    void SetAttribute(Attr attr, bool b);
};

#endif
