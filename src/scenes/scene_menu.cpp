#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "../JsonData.h"

using namespace std;
using namespace sf;

int difficulty = 1;

void MenuScene::Load() {
    cout << "Menu Load \n";
    {
    auto heading = makeEntity();
    auto t = heading->addComponent<TextComponent>(
            "Second Thoughts\n"
            "0 -> Easy || 1 -> Hard (default)\n"
            "Space -> New Game (default) || Enter -> Continue"
            );
    }
    setLoaded(true);
}

void MenuScene::Update(const double& dt) {
    Scene::Update(dt);
    // Handle Difficulty
    if(sf::Keyboard::isKeyPressed(Keyboard::Num1)) {
        difficulty = 1;
        cout << "Hard Mode Chosen" << endl;
    }
    if(sf::Keyboard::isKeyPressed(Keyboard::Num0)) {
        difficulty = 0;
        cout << "Easy Mode Chosen" << endl;
    }

    if(sf::Keyboard::isKeyPressed(Keyboard::Space)){
        JsonData::playerData["difficulty"] = difficulty;
        JsonData::playerData["currentHealth"] = JsonData::playerData["maxHealth"];
        JsonData::playerData["currentLevel"] = 1;
        JsonData::updatePlayerData();
        Engine::ChangeScene(&level1);
        return;
    }

    if (sf::Keyboard::isKeyPressed(Keyboard::Enter)) {
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
