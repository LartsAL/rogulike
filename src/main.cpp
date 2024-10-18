#include <dungeon.h>
#include <iostream>
#include <logger.h>

int main() {
    LOG_INFO("Starting up.")
    DungeonController dc = DungeonController(200, 120, 7, 18, 69);
    dc.generateMap();
    std::cout << dc.toString() << std::endl;
    LOG_INFO("Finished working.")

    return 0;
}
