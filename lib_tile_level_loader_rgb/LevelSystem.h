#pragma once

#include <SFML/Graphics.hpp>
#include <maths.h>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "bmp.h"

#define ls LevelSystem

class LevelSystem {
public:
    static void loadLevelFile(const char*, float tileSize = 100.0f);
    static void unload();
    static void render(sf::RenderWindow& window);

    typedef sf::Color Tile;

    enum TILES {
        EMPTY = sf::Color(255.f, 255.f, 255.f).toInteger(),   // White
        START = sf::Color(0.f, 0.f, 255.f).toInteger(),       // Blue
        WALL = sf::Color(0.f, 255.f, 0.f).toInteger(),        // Green
        END = sf::Color(255.f, 0.f, 0.f).toInteger(),         // Red
        KEY = sf::Color(255.f, 255.f, 0.f).toInteger(),       // Yellow
        GENEMY = sf::Color(255.f, 0.f, 255.f).toInteger(),    // Pink
        TENEMY = sf::Color(0.f, 255.f, 255.f).toInteger(),    // Aqua
        FENEMY = sf::Color(125.f, 125.f, 255.f).toInteger(),  // Purple
        HPICKUP = sf::Color(255.f, 125.f, 125.f).toInteger(), //
    };

    static Tile getTile(sf::Vector2ul);
    static Tile getTileAt(sf::Vector2f);
    static bool isOnGrid(sf::Vector2f);
    static size_t getWidth();
    static size_t getHeight();
    static sf::Vector2f getTilePosition(sf::Vector2ul);
    static std::vector<sf::Vector2ul> findTiles(Tile);
    static sf::Color getColor(Tile t);
    //static void setColor(Tile t, sf::Color c);
    static void setOffset(const sf::Vector2f& _offset);
    static const sf::Vector2f& getOffset();
    static float getTileSize();

protected:
    static std::unique_ptr<Tile[]> _tiles;
    static size_t _width;
    static size_t _height;
    static sf::Vector2f _offset;

    static std::vector<std::unique_ptr<sf::RectangleShape>> _sprites;

    static void buildSprites(bool optimise = true);

    static float _tileSize; // for rendering
    static std::map<Tile, sf::Color> _colours;

private:
    LevelSystem() = delete;
    ~LevelSystem() = delete;
};