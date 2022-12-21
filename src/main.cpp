#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "bmp.h"

using namespace std;

MenuScene menu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;

int main() {
    Engine::Start(1000, 600, "CA3",&menu);

    /*
    BMP bmp = BMP("../../res/Level1.bmp");
    auto pixels = bmp.getColouredPixels();
    for (int i = 0; i < pixels.size(); i++){
        cout << i % bmp.getWidth() << ", " << i / bmp.getWidth() << ": {" << to_string(pixels[i].r) << ", " << to_string(pixels[i].g) << ", " << to_string(pixels[i].b) << "}" << endl;
    }*/
}