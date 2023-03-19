#ifndef GAMEPLAY_ANIM_ANIMATION
#define GAMEPLAY_ANIM_ANIMATION

#include <functional>

enum ControlType
{
    ASSIGN, // Re-assign value
    ERASE,  // Erase last value before add
    MERGE,  // Add value only, assume that the value will be corrected each time
};

class Animation
{
protected:
    double _internalValue = 0;

public:
    // Controlled values are re-assigned. Non-controlled values will respect external changes.
    ControlType control;
    double startTime, endTime;
    double startValue, endValue;
    double *valuePtr;
    std::function<double(double)> timeCurve; // Curve controller

    void tick(double time);
};

#endif /* GAMEPLAY_ANIM_ANIMATION */
