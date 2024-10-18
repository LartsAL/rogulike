#pragma once

#include <memory>
#include <vector>

#define MAX_ROOM_CREATION_ATTEMPTS 100000
#define MAX_SUB_ROOM_CREATION_ATTEMPTS 1000

enum Tile {
    NONE = ' ',
    WALL = '#',
    DOOR = '.',
    FLOOR = '.',
    PLAYER = '@',
    PLACEHOLDER = '?'
};

class Room {
public:
    Room(int x, int y, int width, int height);

    auto intersects(const std::shared_ptr<Room>& other) const -> bool;

    inline auto getX() const noexcept -> int;
    inline auto getY() const noexcept -> int;
    inline auto getWidth() const noexcept -> int;
    inline auto getHeight() const noexcept -> int;
    inline auto getTile(int x, int y) const noexcept -> Tile;
    inline auto setTile(int x, int y, Tile tile) noexcept -> void;
    auto tileHasNeighbour(int x, int y, Tile type) const -> bool;

private:
    int x, y;
    int width, height;
    std::vector<std::vector<Tile>> layout;
    //std::vector<Tile> doors;
};

class DungeonController {
public:
    DungeonController(int width, int height, int roomMinSize, int roomMaxSize, int roomCount);
    ~DungeonController();

    auto generateMap() -> void;
    auto toString() -> std::string;

    inline auto setTile(int x, int y, Tile tile) noexcept -> void;
    inline auto getTile(int x, int y) const noexcept -> Tile;

private:
    std::vector<std::vector<Tile>> map;
    std::vector<std::shared_ptr<Room>> rooms;

    int width, height;
    int roomMinSize, roomMaxSize;
    int roomCount;

    auto placeRandomRoom() -> void;

    auto fillRoomTiles(const std::shared_ptr<Room>& room) -> void;
    auto generateRectRoom(int minSize, int maxSize) -> std::shared_ptr<Room>;
    auto generateCompoundRoom(int minSize, int maxSize) -> std::shared_ptr<Room>;
    auto generateEllipticRoom(int minSize, int maxSize) -> std::shared_ptr<Room>;
    auto canPlaceRoom(const std::shared_ptr<Room>& newRoom) -> bool;
};
