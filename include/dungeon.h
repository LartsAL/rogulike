#pragma once

#include <memory>
#include <vector>

class Dungen {
public:
    enum Tile {
        NONE = ' ',
        WALL = '#',
        DOOR = '.',
        FLOOR = '.',
        PLAYER = '@',
        PLACEHOLDER = '?'
    };

    Dungen(int width, int height);
    ~Dungen();

    auto generateMap() -> void;
    auto toString() -> std::string;

    inline auto setTile(int x, int y, Tile tile) noexcept -> void;
    inline auto getTile(int x, int y) const noexcept -> Tile;

private:
    class Room {
    public:
        Room(int x, int y, int width, int height);
        auto intersects(const std::shared_ptr<Room>& other) const -> bool;

        inline auto setTile(int x, int y, Tile tile) noexcept -> void;
        inline auto getTile(int x, int y) const noexcept -> Tile;
    private:
        int x;
        int y;
        int width;
        int height;
        std::vector<std::vector<Tile>> tiles;
        std::vector<Tile> entrances;
    };

    std::vector<std::vector<Tile>> map;
    std::vector<std::shared_ptr<Room>> rooms;

    int width;
    int height;

    auto fillNone() -> void;
    auto generateRoom(int minSize, int maxSize) -> std::shared_ptr<Room>;
    auto generateRectRoom(int minSize, int maxSize) -> std::shared_ptr<Room>;
    auto generateEllipticRoom(int minSize, int maxSize) -> std::shared_ptr<Room>;
    auto canPlaceRoom(const std::shared_ptr<Room>& newRoom) -> bool;
};
