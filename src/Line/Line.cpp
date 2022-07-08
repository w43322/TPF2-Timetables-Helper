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
