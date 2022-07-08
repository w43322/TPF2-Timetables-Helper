#ifndef TIMING_H
#define TIMING_H

#include <cstdint>

class Time
{
    friend class TimeTable;
public:
    Time(){}
    Time(uint8_t t1, uint8_t t2): mm(t1), ss(t2) {}
private:
    uint8_t mm, ss;
};

class ArrDepTime
{
    friend class TimeTable;
public:
    ArrDepTime(uint8_t t1, uint8_t t2, uint8_t t3, uint8_t t4): arr(t1, t2), dep(t3, t4) {}
private:
    Time arr, dep;
};

#endif
