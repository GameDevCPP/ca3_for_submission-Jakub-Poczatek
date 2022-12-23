#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "../JsonData.h"

using namespace std;
using namespace sf;

void MenuScene::Load() {
    cout << "Menu Load \n";
    {
    auto txt = makeEntity();
    auto t = txt->addComponent<TextComponent>(
        "Platformer\nPress Space to Start");
    }
    setLoaded(true);
}

void MenuScene::Update(const double& dt) {
    Scene::Update(dt);
    if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
        switch((int) JsonData::playerData["currentLevel"]){
            case 1:
                Engine::ChangeScene(&level1);
                return;
            case 2:
                Engine::ChangeScene(&level2);
                return;
            case 3:
                Engine::ChangeScene(&level3);
                return;
            case 0:
                Engine::ChangeScene(&levelTest);
                return;
        }
    }
}
