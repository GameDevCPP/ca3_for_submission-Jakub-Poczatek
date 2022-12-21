#include "LevelSystem.h"
#include <fstream>

using namespace std;
using namespace sf;

sf::Color LevelSystem::getColor(LevelSystem::Tile t) {
    return Color(t);
}

std::unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;

float LevelSystem::_tileSize(100.f);
Vector2f LevelSystem::_offset(0.0f, 30.0f);
vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

void LevelSystem::loadLevelFile(const char* path, float tileSize) {
    BMP bmp = BMP(path);
    _tileSize = tileSize;
    _width = bmp.getWidth();
    _height = bmp.getHeight();
    _tiles = std::make_unique<Tile[]>(bmp.getWidth() * bmp.getHeight());
    vector<sf::Color> colouredPixels = bmp.getColouredPixels();
    std::copy(colouredPixels.begin(), colouredPixels.end(), &_tiles[0]);
}

void LevelSystem::buildSprites(bool optimise) {
    _sprites.clear();

    struct tp {
        sf::Vector2f p, s;
        sf::Color c;
    };
    vector<tp> tps;
    const auto tls = Vector2f(_tileSize, _tileSize);
    for (size_t y = 0; y < _height; ++y) {
        for (size_t x = 0; x < _width; ++x){
            Tile t = getTile({x, y});
            if ((t.toInteger() != WALL) && (t.toInteger() != END)) {
                continue;
            }
            tps.push_back({getTilePosition({x, y}), tls, t});
        }
    }

    const auto nonEmpty = tps.size();

    // If tile of the same type are next to each other,
    // we can use one large sprite isntead of two.
    if(optimise && nonEmpty) {
        vector<tp> tpo;
        tp last = tps[0];
        size_t sameCount = 0;

        for(size_t i = 1; i < nonEmpty; ++i) {
            // Is this tile compressible with the last?
            bool same = ((tps[i].p.y == last.p.y) &&
                    (tps[i].p.x == last.p.x + (tls.x * (1 + sameCount))) &&
                    (tps[i].c == last.c));
            if(same) {
                ++sameCount; // Yes, keep going
            } else {
                if (sameCount){
                    last.s.x = (1 + sameCount) * tls.x; // Expand tile
                }
                // Write tile to list
                tpo.push_back(last);
                sameCount = 0;
                last = tps[i];
            }
        }
        // Catch the last tile
        if (sameCount) {
            last.s.x = (1 + sameCount) * tls.x;
            tpo.push_back(last);
        }

        // No scan down Y, using different algo now that compressible blocks may
        // not be contiguous.
        const auto xSave = tpo.size();
        sameCount = 0;
        vector<tp> tpox;
        for (size_t i = 0; i < tpo.size(); ++i) {
            last = tpo[i];
            for (size_t j = 0; j < tpo.size(); ++j){
                bool same = ((tpo[j].p.x == last.p.x) && (tpo[j].s == last.s) &&
                        (tpo[j].p.y == last.p.y + (tls.y * (1 + sameCount))) &&
                        (tpo[j].c == last.c));
                if (same){
                    ++sameCount;
                    tpo.erase(tpo.begin() + j);
                    --j;
                }
            }
            if(sameCount) {
                last.s.y = (1 + sameCount) * tls.y; //Expand tile
            }

            // Write tile to list
            tpox.push_back(last);
            sameCount = 0;
        }
        tps.swap(tpox);
    }

    for (auto& t: tps) {
        auto s = make_unique<sf::RectangleShape>();
        s->setPosition(t.p);
        s->setSize(t.s);
        s->setFillColor(t.c);
        _sprites.push_back(move(s));
    }

    cout << "Level with " << (_width * _height) << " Tiles, With " << nonEmpty <<
    " Non Empty, using: " << _sprites.size() << " Sprites" << endl;
}

void LevelSystem::render(RenderWindow& window){
    for (auto& t: _sprites) {
        window.draw(*t);
    }
}

LevelSystem::Tile LevelSystem::getTile(sf::Vector2ul p) {
    if (p.x > _width || p.y > _height){
        throw string("Tile out of range: ") + to_string(p.x) + "," +
                to_string(p.y);
    }
    return _tiles[(p.y * _width) + p.x];
}

size_t LevelSystem::getWidth() {
    return _width;
}

size_t LevelSystem::getHeight() {
    return _height;
}

sf::Vector2f LevelSystem::getTilePosition(sf::Vector2ul p){
    return (Vector2f(p.x, p.y) * _tileSize) + _offset;
}

std::vector<sf::Vector2ul> LevelSystem::findTiles(LevelSystem::Tile type){
    auto v = vector<sf::Vector2ul>();
    for (size_t i = 0; i < _width * _height; ++i){
        if (_tiles[i] == type){
            v.emplace_back(i % _width, i / _width);
        }
    }
    return v;
}

LevelSystem::Tile LevelSystem::getTileAt(sf::Vector2f v) {
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0) {
        throw string("Tile out of range ");
    }
    return getTile(Vector2ul((v - _offset) / (_tileSize)));
}

bool LevelSystem::isOnGrid(sf::Vector2f v) {
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0){
        return false;
    }
    auto p = Vector2ul((v - _offset) / (_tileSize));
    if (p.x > _width || p.y > _height) {
        return false;
    }
    return true;
}

void LevelSystem::setOffset(const sf::Vector2f &_offset) {
    LevelSystem::_offset = _offset;
    buildSprites();
}

void LevelSystem::unload() {
    cout << "LevelSystem unloading\n";
    _sprites.clear();
    _tiles.reset();
    _width = 0;
    _height = 0;
    _offset = {0, 0};
}

const Vector2f& LevelSystem::getOffset() {
    return _offset;
}

float LevelSystem::getTileSize() {
    return _tileSize;
}