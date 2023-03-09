#include "Animation.hh"

void Animation::tick(double time)
{
    if (!control)
    {
        *valuePtr -= _internalValue;
    }
    _internalValue = timeCurve((time - startTime) / (endTime - startTime)) * (endValue - startValue) + startValue;
    if (control)
    {
        *valuePtr = _internalValue;
    }
    else
    {
        *valuePtr += _internalValue;
    }
}