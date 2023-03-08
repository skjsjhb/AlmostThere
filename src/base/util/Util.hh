#ifndef BASE_UTIL_UTIL
#define BASE_UTIL_UTIL

bool isOverlapped(double p1, double p2, double range1, double range2);
void vecmov(const float *src, float *dest, unsigned int deg);
void matmov(const void *src, void *dest, unsigned int a);

#endif /* BASE_UTIL_UTIL */
