#include "Util.hh"
#include <cstring>
#include <cmath>

bool isOverlapped(double p1, double r1, double p2, double r2)
{
    return abs(p2 - p1) <= abs(r1) + abs(r2);
}
