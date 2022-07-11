#include "Timing.h"

Time Time::operator +(int s)
{
    ss += s;
    if (s < 0)
        ss += 3600;
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

bool Time::operator <(const Time &t) const
{
    return mm < t.mm || (mm == t.mm && ss < t.ss);
}

bool ArrDepTime::operator <(const ArrDepTime &t) const
{
    return dep < t.dep || (dep == t.dep && arr < t.arr);
}

bool Time::operator ==(const Time &t) const
{
    return mm == t.mm && ss == t.ss;
}

bool ArrDepTime::operator ==(const ArrDepTime &t) const
{
    return dep == t.dep && arr == t.arr;
}

int Time::Seconds()
{
    return mm * 60 + ss;
}
