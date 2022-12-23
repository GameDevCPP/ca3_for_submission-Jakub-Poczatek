#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "bmp.h"
#include <SFML/Audio.hpp>

using namespace std;

MenuScene menu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
LevelTestScene levelTest;

int main() {
    Engine::Start(1280, 720, "CA3",&menu);
    //sf::SoundBuffer buffer;
    //if (!buffer.loadFromFile())
}