#include "TestTools.hh"

#include "gameplay/anim/Animation.hh"

double linear(double v)
{
    return v;
}

double constant(double v)
{
    return 1;
}

int main()
{
    Animation a;
    double x;
    a.valuePtr = &x;
    a.control = true;
    a.startValue = 0;
    a.endValue = 1;
    a.startTime = 0;
    a.endTime = 50;
    a.timeCurve = linear;
    a.tick(25);
    WANT(x == 0.5);
    a.tick(50);
    WANT(x == 1);

    Animation b, c, d;
    double y = 1;
    b.valuePtr = &y;
    c.valuePtr = &y;
    d.valuePtr = &y;
    b.control = false;
    c.control = false;
    d.control = false;
    b.startTime = 0;
    b.endTime = 100;
    c.startTime = 50;
    c.endTime = 150;
    d.startTime = 75;
    d.endTime = 150;
    b.startValue = 1;
    b.endValue = 2;
    c.startValue = -2;
    c.endValue = -10;
    d.startValue = -1;
    d.endValue = 1;
    b.timeCurve = linear;
    c.timeCurve = linear;
    d.timeCurve = constant;

    b.tick(25);
    c.tick(25);
    d.tick(25);
    WANT(y == 3.25);

    b.tick(100);
    c.tick(100);
    d.tick(100);
    WANT(y == -2);

    TEND;
}