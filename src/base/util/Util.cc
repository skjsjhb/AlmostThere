#include "Util.hh"
#include <cstring>

bool isOverlapped(double p1, double p2, double range1, double range2)
{
    return ((p1 + range1) >= (p2 - range2)) || ((p1 - range1) <= (p2 + range2));
}

void vecmov(const float *src, float *dest, unsigned int deg)
{
    memcpy(dest, src, deg * sizeof(float));
}

void matmov(const void *src, void *dest, unsigned int a)
{
    memcpy(dest, src, a * a * sizeof(float));
}