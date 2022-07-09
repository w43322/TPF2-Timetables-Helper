#include "Timing.h"

Time Time::operator +(int s)
{
    ss += s;
    Normalize();
    return *this;
}

Time Time::operator +(const Time &t)
{
    ss += t.ss;
    mm += t.mm;
    Normalize();
    return *this;
}

void Time::Normalize()
{
    mm += ss / 60;
    ss %= 60;
    mm %= 60;
}
