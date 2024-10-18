#include "dungen.h"

#include "utils.h"

Dungen::Room::Room(const int x, const int y, const int width, const int height):
    x(x), y(y), width(width), height(height), tiles(height, std::vector<Tile>(width)) {}

auto Dungen::Room::intersects(const std::shared_ptr<Room>& other) const -> bool {
    return this->x <= other->x + other->width + 1 &&
           this->x + this->width + 1 >= other->x &&
           this->y <= other->y + other->height + 1 &&
           this->y + this->height + 1 >= other->y;
}

auto Dungen::Room::setTile(const int x, const int y, const Tile tile) noexcept -> void {
    tiles[y][x] = tile;
}

auto Dungen::Room::getTile(const int x, const int y) const noexcept -> Tile {
    return tiles[y][x];
}

Dungen::Dungen(const int width, const int height):
    width(width), height(height) {}

Dungen::~Dungen() = default;

auto Dungen::generateMap() -> void {
    fillNone();
}

auto Dungen::toString() -> std::string {
    std::string str;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            str += std::to_string(getTile(x, y));
        }
        str += '\n';
    }
    return str;
}

auto Dungen::setTile(const int x, const int y, const Tile tile) noexcept -> void {
    map[y][x] = tile;
}

auto Dungen::getTile(const int x, const int y) const noexcept -> Tile {
    return map[y][x];
}

auto Dungen::fillNone() -> void {
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[0].size(); x++) {
            map[y][x] = NONE;
        }
    }
}

auto Dungen::generateRoom(const int minSize, const int maxSize) -> std::shared_ptr<Room> {
    int type = randomInt(0, 100);
    if (type >= 0 && type < 70) {
        generateRectRoom(minSize, maxSize);
    } else if (type >= 70 && type < 100) {
        generateEllipticRoom(minSize, maxSize);
    } else if (type == 100) {

    }
}

auto Dungen::generateRectRoom(const int minSize, const int maxSize) -> std::shared_ptr<Room> {
    int roomWidth = randomInt(minSize, maxSize);
    int roomHeight = randomInt(minSize, maxSize);

    int x = randomInt(0, width - roomWidth);
    int y = randomInt(0, height - roomHeight);

    const auto room = std::make_shared<Room>(x, y, roomWidth, roomHeight);
    if (canPlaceRoom(room)) {
        return nullptr;
    }

    // Insides
    for (int y = 1; y < roomHeight - 1; y++) {
        for (int x = 1; x < roomWidth - 1; x++) {
            room->setTile(x, y, FLOOR);
        }
    }
    // Walls
    for (int x = 0; x < roomWidth; x++) {
        room->setTile(x, 0, WALL);
        room->setTile(x, roomHeight - 1, WALL);
    }
    for (int y = 0; y < roomHeight; y++) {
        room->setTile(0, y, WALL);
        room->setTile(roomWidth - 1, y, WALL);
    }

    return room;
}

auto Dungen::generateEllipticRoom(int minSize, int maxSize) -> std::shared_ptr<Room> {

}

auto Dungen::canPlaceRoom(const std::shared_ptr<Room>& newRoom) -> bool {
    for (const auto& room : rooms) {
        if (room->intersects(newRoom)) {
            return false;
        }
    }
    return true;
}
