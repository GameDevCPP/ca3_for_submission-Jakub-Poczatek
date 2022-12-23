#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "bmp.h"
using namespace std;

MenuScene menu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
LevelTestScene levelTest;

int main() {
    Audio::start("../../res/audio/background.wav");
    Engine::Start(1280, 720, "CA3",&menu);
}