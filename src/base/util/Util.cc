#include "Util.hh"
#include <cstring>
#include <cmath>

bool isOverlapped(double p1, double r1, double p2, double r2)
{
    return abs(p2 - p1) <= abs(r1) + abs(r2);
}

void vecmov(const float *src, float *dest, unsigned int deg)
{
    memcpy(dest, src, deg * sizeof(float));
}

void matmov(const void *src, void *dest, unsigned int a)
{
    memcpy(dest, src, static_cast<long>(a) * a * sizeof(float));
}