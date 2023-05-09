#ifndef BASE_UTIL_UTIL
#define BASE_UTIL_UTIL

#include <vector>
#include <string>

bool isOverlapped(double p1, double r1, double p2, double r2);

std::vector<std::string> splitStr(const std::string &src, const std::string &del, int maxn = INT_MAX);

#endif /* BASE_UTIL_UTIL */
