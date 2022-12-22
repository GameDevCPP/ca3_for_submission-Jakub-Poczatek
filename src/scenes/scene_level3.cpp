#include "scene_level3.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_key.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_enemy_aStar.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> key;
static vector<shared_ptr<Entity>> enemies;
static vector<shared_ptr<Entity>> turrets;

void Level3Scene::Load() {
    cout << " Scene 3 Load" << endl;

    ls::loadLevelFile("../../res/Level3.bmp", 40.0f);

    //auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(0, 0));

    // Create player
    {
        player = makeEntity();
        player->setPosition(ls::getTilePosition(ls::findTiles(Color(ls::START))[0]));
        auto s = player->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
        s->getShape().setFillColor(Color::Magenta);
        s->getShape().setOrigin(Vector2f(10.f, 15.f));

        player->addTag("player");
        player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
    }

    // Create Key Pickup

    key = makeEntity();
    key->setPosition(ls::getTilePosition(ls::findTiles(Color(ls::KEY))[0]) +
                     Vector2f(20, 20));
    key->addComponent<KeyComponent>();
    auto s = key->addComponent<ShapeComponent>();
    s->setShape<sf::CircleShape>(20.f);
    s->getShape().setFillColor(Color::Yellow);
    s->getShape().setOrigin({20.f, 20.f});

    // Create Ground Enemies
    for(auto tile: ls::findTiles(Color(ls::GENEMY))){
        auto enemy = makeEntity();
        enemy->setPosition(ls::getTilePosition(tile) + Vector2f(0, 24));
        //enemy->setPosition(Vector2f(ls::getWidth()*20/2, ls::getHeight()*20/2));
        enemy->addComponent<HurtComponent>();
        auto s = enemy->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(16.f);
        s->getShape().setFillColor(Color::Red);
        s->getShape().setOrigin(Vector2f(16.f, 16.f));
        enemy->addComponent<EnemyAIComponent>();
        enemies.push_back(enemy);
    }

    // Create Flying Enemies
    for(auto tile: ls::findTiles(Color(ls::FENEMY))){
        auto enemy = makeEntity();
        enemy->setPosition(ls::getTilePosition(tile) + Vector2f(0, 0));
        //enemy->setPosition(Vector2f(ls::getWidth()*20/2, ls::getHeight()*20/2));
        enemy->addComponent<HurtComponent>();
        auto s = enemy->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(8.f);
        s->getShape().setFillColor(Color::Cyan);
        s->getShape().setOrigin(Vector2f(8.f, 8.f));
        enemy->addComponent<EnemyAStarComponent>();
        enemies.push_back(enemy);
    }

    // Create Turrets
    for (auto tile: ls::findTiles(Color(ls::TENEMY))){
        auto turret = makeEntity();
        turret->setPosition(ls::getTilePosition(tile) + Vector2f(20, 0));
        auto s = turret->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(16.f, 3);
        s->getShape().setFillColor(Color::Red);
        s->getShape().setOrigin(Vector2f(16.f, 16.f));
        turret->addComponent<EnemyTurretComponent>();

        turrets.push_back(turret);
    }

    // Add physics colliders to level tiles.
    {
        auto walls = ls::findTiles(sf::Color(ls::WALL));
        for (auto w : walls) {
            auto pos = ls::getTilePosition(w);
            pos += Vector2f(20.f, 20.f); //offset to center
            auto e = makeEntity();
            e->setPosition(pos);
            e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
        }
    }

    //Simulate long loading times
    //std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    cout << " Scene 3 Load Done" << endl;
    setLoaded(true);
}

void Level3Scene::UnLoad() {
    cout << "Scene 3 Unload" << endl;
    player.reset();
    ls::unload();
    Scene::UnLoad();
}

void Level3Scene::Update(const double& dt) {
    sf::View view(sf::FloatRect({0.f, 0.f}, {500.f, 350.f}));
    view.setCenter({player->getPosition().x, player->getPosition().y});
    Engine::GetWindow().setView(view);
    Scene::Update(dt);
    if (ls::getTileAt(player->getPosition()) == Color(ls::END) && !key->isAlive()) {
        Engine::ChangeScene((Scene*)&level1);
    }
    if(!player->isAlive()){
        Engine::ChangeScene((Scene*)&level3);
    }
}

void Level3Scene::Render() {
    ls::render(Engine::GetWindow());
    Scene::Render();
}
