#ifndef BASE_UTIL_UTIL
#define BASE_UTIL_UTIL

#include <vector>
#include <string>
#include <climits>

bool isOverlapped(double p1, double r1, double p2, double r2);

std::vector<std::string> splitStr(const std::string &src, const std::string &del, int maxn = INT_MAX);

/**
 * @brief Trim a string from both left and right in-place.
 * @param[in,out] s String to be trimmed
 */
void trimStr(std::string &s);

template <typename T>
std::vector<T> strip(const std::vector<T> &src, const std::vector<unsigned int> ind)
{
    std::vector<T> c;
    for (auto &i : ind)
    {
        c.push_back(src[i]);
    }
    return c;
};

template <typename T>
std::vector<T> strip(const T *src, const std::vector<unsigned int> ind)
{
    std::vector<T> c;
    for (auto &i : ind)
    {
        c.push_back(src[i]);
    }
    return c;
};

#endif /* BASE_UTIL_UTIL */
