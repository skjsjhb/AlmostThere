#include "Animation.hh"

void Animation::tick(double time)
{
    if (control == ERASE)
    {
        *valuePtr -= _internalValue;
    }
    _internalValue = timeCurve((time - startTime) / (endTime - startTime)) * (endValue - startValue) + startValue;
    if (control == ASSIGN)
    {
        *valuePtr = _internalValue;
    }
    else
    {
        *valuePtr += _internalValue;
    }
}