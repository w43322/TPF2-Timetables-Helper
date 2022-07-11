#ifndef TIMING_H
#define TIMING_H

#include <cstdint>

class Time
{
    friend class TimeTable;
public:
    Time(){}
    Time(int t1, int t2): mm(t1), ss(t2) {}
    Time operator +(int s);
    Time operator +(const Time &t);
    bool operator <(const Time &t) const;
    bool operator ==(const Time &t) const;
    int Seconds();
private:
    int mm, ss;
    void Normalize();
};

class ArrDepTime
{
    friend class TimeTable;
public:
    ArrDepTime(int t1, int t2, int t3, int t4): arr(t1, t2), dep(t3, t4) {}
    bool operator <(const ArrDepTime &t) const;
    bool operator ==(const ArrDepTime &t) const;
private:
    Time arr, dep;
};

#endif
