#include "Util.hh"
#include <cstring>
#include <cmath>

bool isOverlapped(double p1, double r1, double p2, double r2)
{
    return std::abs(p2 - p1) <= std::abs(r1) + std::abs(r2);
}
