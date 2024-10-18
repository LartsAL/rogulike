#include "utils.h"

#include <cstdlib>  // rand

auto randomInt(int min, int max) -> int {
    return rand() % (max - min + 1) + min;
}

