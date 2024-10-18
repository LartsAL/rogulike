#include "utils.h"

#include <random>

auto randomInt(int min, int max) -> int {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}
