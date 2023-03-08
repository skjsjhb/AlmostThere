#ifndef TESTTOOLS
#define TESTTOOLS

#include <iostream>

static bool __TEST_ERR = false;
#define WANT(c)                \
    {                          \
        if (!(c))              \
        {                      \
            __TEST_ERR = true; \
        }                      \
    }

#define TEND                       \
    {                              \
        return __TEST_ERR ? 1 : 0; \
    }

#define DEBUG(x)                       \
    {                                  \
        std::cout << (x) << std::endl; \
    }

#endif /* TESTTOOLS */
