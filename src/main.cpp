#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "JsonData.h"
using namespace std;

MenuScene menu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
LevelTestScene levelTest;

Json JsonData::playerData;
Json JsonData::generalData;
Json JsonData::pickups;
Json JsonData::enemies;
Json JsonData::audio;

int main() {
    JsonData::start();
    Audio::start(JsonData::audio["bg"]);
    Engine::Start(1920, 1080, "CA3",&menu);
}