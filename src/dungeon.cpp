#include "dungeon.h"

#include <iostream>

#include "utils.h"
#include "logger.h"

Room::Room(const int x, const int y, const int width, const int height):
    x(x), y(y), width(width), height(height), layout(height, std::vector(width, NONE)) {}

auto Room::intersects(const std::shared_ptr<Room>& other) const -> bool {
    return this->x <= other->x + other->width + 1 &&
           this->x + this->width + 1 >= other->x &&
           this->y <= other->y + other->height + 1 &&
           this->y + this->height + 1>= other->y;
}

auto Room::getX() const noexcept -> int {
    return this->x;
}

auto Room::getY() const noexcept -> int {
    return this->y;
}

auto Room::getWidth() const noexcept -> int {
    return this->width;
}

auto Room::getHeight() const noexcept -> int {
    return this->height;
}

auto Room::setTile(const int x, const int y, const Tile tile) noexcept -> void {
    layout[y][x] = tile;
}

auto Room::getTile(const int x, const int y) const noexcept -> Tile {
    return layout[y][x];
}

auto Room::tileHasNeighbour(int x, int y, Tile type) const -> bool {
    // Assume that if it is a boundary tile it always have NONE neighbour
    if ((x == 0 || x == width - 1 || y == 0 || y == height - 1) && type == NONE) {
        return true;
    }

    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if ((i >= 0 && i < height && j >= 0 && j < width) && (i != y && j != x) && layout[i][j] == type) {
                return true;
            }
        }
    }
    return false;
}

DungeonController::DungeonController(int width, int height, int roomMinSize, int roomMaxSize, int roomCount):
    width(width), height(height), roomMinSize(roomMinSize), roomMaxSize(roomMaxSize), roomCount(roomCount) {}

DungeonController::~DungeonController() = default;

auto DungeonController::generateMap() -> void {
    map = std::vector(height, std::vector(width, NONE));

    for (int i = 0; i < roomCount; ++i) {
        placeRandomRoom();
    }
}

auto DungeonController::toString() -> std::string {
    std::string str;
    int width = map[0].size();
    int height = map.size();
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            str += std::string(3, static_cast<char>(getTile(x, y)));
        }
        str += '\n';
    }
    return str;
}

auto DungeonController::setTile(const int x, const int y, const Tile tile) noexcept -> void {
    map[y][x] = tile;
}

auto DungeonController::getTile(const int x, const int y) const noexcept -> Tile {
    return map[y][x];
}

auto DungeonController::placeRandomRoom() -> void {
    int type = randomInt(0, 100);
    std::shared_ptr<Room> room;
    if (type >= 0 && type < 70) {
        // room = generateRectRoom(roomMinSize, roomMaxSize);
        room = generateCompoundRoom(roomMinSize, roomMaxSize);
        LOG_DEBUG("Generated rectangular room.")
    } else if (type >= 70 && type < 100) {
        room = generateEllipticRoom(roomMinSize, roomMaxSize);
        LOG_DEBUG("Generated elliptic room.")
    } else if (type == 100) {
        LOG_DEBUG("Got into special room case. Nothing happened.")
    }

    if (room != nullptr) {
        fillRoomTiles(room);
        rooms.push_back(room);
    } else {
        LOG_WARNING("Can not place room.")
    }
}

auto DungeonController::fillRoomTiles(const std::shared_ptr<Room>& room) -> void {
    for (int i = 0; i < room->getHeight(); ++i) {
        for (int j = 0; j < room->getWidth(); ++j) {
            int x = room->getX() + j;
            int y = room->getY() + i;
            if (x < width && y < height) {
                map[y][x] = room->getTile(j, i);
            }
        }
    }
}

auto DungeonController::generateRectRoom(const int minSize, const int maxSize) -> std::shared_ptr<Room> {
    bool placed = false;

    int x, y;
    int roomWidth, roomHeight;
    std::shared_ptr<Room> newRoom;

    for (int i = 0; i < MAX_ROOM_CREATION_ATTEMPTS; ++i) {
        roomWidth = randomInt(minSize, maxSize);
        roomHeight = randomInt(minSize, maxSize);

        x = randomInt(0, width - roomWidth);
        y = randomInt(0, height - roomHeight);

        newRoom = std::make_shared<Room>(x, y, roomWidth, roomHeight);
        if (canPlaceRoom(newRoom)) {
            placed = true;
            break;
        }
    }

    if (!placed) {
        return nullptr;
    }

    // Floor
    for (int i = 1; i < roomHeight - 1; ++i) {
        for (int j = 1; j < roomWidth - 1; ++j) {
            newRoom->setTile(j, i, FLOOR);
        }
    }
    // Walls
    for (int i = 0; i < roomWidth; ++i) {
        newRoom->setTile(i, 0, WALL);
        newRoom->setTile(i, roomHeight - 1, WALL);
    }
    for (int i = 0; i < roomHeight; ++i) {
        newRoom->setTile(0, i, WALL);
        newRoom->setTile(roomWidth - 1, i, WALL);
    }

    return newRoom;
}

auto DungeonController::generateCompoundRoom(int minSize, int maxSize) -> std::shared_ptr<Room> {
    bool placed = false;

    int x, y;
    int roomWidth, roomHeight;
    std::shared_ptr<Room> newRoom;

    for (int i = 0; i < MAX_ROOM_CREATION_ATTEMPTS; ++i) {
        roomWidth = randomInt(minSize, maxSize);
        roomHeight = randomInt(minSize, maxSize);

        x = randomInt(0, width - roomWidth);
        y = randomInt(0, height - roomHeight);

        newRoom = std::make_shared<Room>(x, y, roomWidth, roomHeight);
        if (canPlaceRoom(newRoom)) {
            placed = true;
            break;
        }
    }

    if (!placed) {
        return nullptr;
    }

    std::vector<std::shared_ptr<Room>> subRooms;
    const int subRects = randomInt(2, 5);

    for (int i = 0; i < subRects; ++i) {
        bool subPlaced = false;

        int subX, subY;
        int subRoomWidth, subRoomHeight;
        for (int j = 0; j < MAX_SUB_ROOM_CREATION_ATTEMPTS; ++j) {
            subRoomWidth = randomInt(roomWidth / 2, roomWidth / 1.5);
            subRoomHeight = randomInt(roomHeight / 2, roomHeight / 1.5);

            subX = randomInt(0, roomWidth - subRoomWidth);
            subY = randomInt(0, roomHeight - subRoomHeight);

            // Place subRect so that it intersect 1+ existing ones
            const auto subRoom = std::make_shared<Room>(subX, subY, subRoomWidth, subRoomHeight);
            if (!subRooms.empty()) {
                for (const auto& room : subRooms) {
                    if (room->intersects(subRoom)) {
                        subRooms.push_back(subRoom);
                        subPlaced = true;
                        break;
                    }
                }
            } else {
                subRooms.push_back(subRoom);
                subPlaced = true;
            }
        }

        if (!subPlaced) {
            return nullptr;
        }

        // Floor
        for (int j = subY + 1; j < subY + subRoomHeight - 1; ++j) {
            for (int k = subX + 1; k < subX + subRoomWidth - 1; ++k) {
                newRoom->setTile(k, j, FLOOR);
            }
        }
        // Walls
        for (int j = subX; j < subX + subRoomWidth; ++j) {
            if (newRoom->getTile(j, subY) == NONE) {
                newRoom->setTile(j, subY, WALL);
            }
            if (newRoom->getTile(j, subY + subRoomHeight - 1) == NONE) {
                newRoom->setTile(j, subY + subRoomHeight - 1, WALL);
            }
        }
        for (int j = subY; j < subY + subRoomHeight; ++j) {
            if (newRoom->getTile(subX, j) == NONE) {
                newRoom->setTile(subX, j, WALL);
            }
            if (newRoom->getTile(subX + subRoomWidth - 1, j) == NONE) {
                newRoom->setTile(subX + subRoomWidth - 1, j, WALL);
            }
        }
    }

    // Additional check to cut holes in solid walls, if they divided the room
    for (int i = 0; i < roomHeight; ++i) {
        for (int j = 0; j < roomWidth; ++j) {
            const Tile tile = newRoom->getTile(j, i);
            if (tile == WALL && newRoom->tileHasNeighbour(j, i, FLOOR) && !newRoom->tileHasNeighbour(j, i, NONE)) {
                newRoom->setTile(j, i, FLOOR);
            }
        }
    }

    return newRoom;
}

auto DungeonController::generateEllipticRoom(int minSize, int maxSize) -> std::shared_ptr<Room> {
    bool placed = false;

    int x, y;
    int roomWidth, roomHeight;
    std::shared_ptr<Room> newRoom;

    for (int i = 0; i < MAX_ROOM_CREATION_ATTEMPTS; ++i) {
        roomWidth = randomInt(minSize, maxSize);
        roomHeight = randomInt(minSize, maxSize);

        // Ensure odd dimensions to avoid problems with the center position
        roomWidth += (roomWidth % 2 ? 0 : 1);
        roomHeight += (roomHeight % 2 ? 0 : 1);

        x = randomInt(0, width - roomWidth);
        y = randomInt(0, height - roomHeight);

        newRoom = std::make_shared<Room>(x, y, roomWidth, roomHeight);
        if (canPlaceRoom(newRoom)) {
            placed = true;
            break;
        }
    }

    if (!placed) {
        return nullptr;
    }

    int centerX = roomWidth / 2;
    int centerY = roomHeight / 2;

    int radiusX = roomWidth / 2;
    int radiusY = roomHeight / 2;
    int radiusX2 = radiusX * radiusX;
    int radiusY2 = radiusY * radiusY;

    // Floor
    for (int i = 0; i < roomHeight; ++i) {
        for (int j = 0; j < roomWidth; ++j) {
            int dx = j - centerX;
            int dy = i - centerY;

            // If point is inside the ellipse
            if ((dx * dx * radiusY2 + dy * dy * radiusX2) < (radiusX2 * radiusY2)) {
                newRoom->setTile(j, i, FLOOR);
            }
        }
    }

    // Walls
    for (int i = 0; i < roomHeight; ++i) {
        for (int j = 0; j < roomWidth; ++j) {
            int dx = j - centerX;
            int dy = i - centerY;

            // If point is outside the ellipse
            if ((dx * dx * radiusY2 + dy * dy * radiusX2) >= (radiusX2 * radiusY2)) {
                // Check neighbours: if any neighbouring cell is inside the ellipse, make the cell a WALL
                if ((j > 0 && newRoom->getTile(j - 1, i) == FLOOR) ||
                    (j < roomWidth - 1 && newRoom->getTile(j + 1, i) == FLOOR) ||
                    (i > 0 && newRoom->getTile(j, i - 1) == FLOOR) ||
                    (i < roomHeight - 1 && newRoom->getTile(j, i + 1) == FLOOR)) {
                    newRoom->setTile(j, i, WALL);
                }
            }
        }
    }

    return newRoom;
}


auto DungeonController::canPlaceRoom(const std::shared_ptr<Room>& newRoom) -> bool {
    for (const auto& room : rooms) {
        if (room->intersects(newRoom)) {
            return false;
        }
    }
    return true;
}
