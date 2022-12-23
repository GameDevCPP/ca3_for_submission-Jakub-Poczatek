#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

sf::Music music;

void MenuScene::Load() {
    cout << "Menu Load \n";
    {
    auto txt = makeEntity();
    auto t = txt->addComponent<TextComponent>(
        "Platformer\nPress Space to Start");
    }
    setLoaded(true);
    if (!music.openFromFile("../../res/audio/background.wav"))
        cout << "Audio not loaded" << endl;
    music.play();
    music.setLoop(true);
    music.setVolume(10.f);
}

void MenuScene::Update(const double& dt) {
    // cout << "Menu Update "<<dt<<"\n";

    if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
    Engine::ChangeScene(&levelTest);
    }

    Scene::Update(dt);
}
