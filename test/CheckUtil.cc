#include "TestTools.hh"

#include "util/Util.hh"

int main() {
    WANT(isOverlapped(0, 10, 15, 10));
    WANT(!isOverlapped(20, 5, 5, 1));
    WANT(isOverlapped(5, 2, 2, 2));
    WANT(isOverlapped(3, 1, 7, 100));
    WANT(!isOverlapped(0, 3, 6, 2));
    WANT(!isOverlapped(2.0, 0.2, 2.9, 0.0));
    TEND
}