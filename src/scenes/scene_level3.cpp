#include "scene_level3.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_pickup.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_enemy_aStar.h"
#include "../components/cmp_entity_health.h"
#include "../components/cmp_health_pickup.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_enemy_ai_targeted.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> key;
static vector<shared_ptr<Entity>> enemies;

void Level3Scene::Load() {
    cout << " Scene 3 Load" << endl;

    ls::loadLevelFile("../../res/levelFiles/level3.bmp", (float) JsonData::generalData["tileSize"]);
    ls::setOffset(Vector2f(0, 0));

    // Create player
    {
        player = makeEntity();
        player->setPosition(ls::getTilePosition(ls::findTiles(Color(ls::START))[0]));
        auto s = player->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(JsonData::playerData["width"],
                                                 JsonData::playerData["height"]));
        s->getShape().setFillColor(Color(ls::START));
        s->getShape().setOrigin(Vector2f((float) JsonData::playerData["width"]/2,
                                         (float) JsonData::playerData["height"]/2));

        player->addTag("player");
        player->addComponent<PlayerPhysicsComponent>(Vector2f(JsonData::playerData["width"],
                                                              JsonData::playerData["height"]));
        player->addComponent<EntityHealth>(JsonData::playerData["currentHealth"]);
    }

    // Create Key Pickup
    {
        key = makeEntity();
        key->setPosition(ls::getTilePosition(ls::findTiles(Color(ls::KEY))[0]) +
                         Vector2f(
                                 (float) JsonData::generalData["tileSize"] - (float) JsonData::pickups["key"]["radius"],
                                 (float) JsonData::generalData["tileSize"] -
                                 (float) JsonData::pickups["key"]["radius"]));
        key->addComponent<PickupComponent>();
        auto s = key->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(JsonData::pickups["key"]["radius"]);
        s->getShape().setFillColor(Color(ls::KEY));
        s->getShape().setOrigin({JsonData::pickups["key"]["radius"],
                                 JsonData::pickups["key"]["radius"]});
    }

    // Create Health Pickups
    for(auto tile: ls::findTiles(Color(ls::HPICKUP))){
        auto health = makeEntity();
        health->setPosition(ls::getTilePosition(tile) +
                            Vector2f((float) JsonData::generalData["tileSize"] - (float) JsonData::pickups["health"]["radius"],
                                     (float) JsonData::generalData["tileSize"] - (float) JsonData::pickups["health"]["radius"]));
        health->addComponent<HealthPickup>();
        auto s = health->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(JsonData::pickups["health"]["radius"]);
        s->getShape().setFillColor(Color(ls::HPICKUP));
        s->getShape().setOrigin({JsonData::pickups["health"]["radius"],
                                 JsonData::pickups["health"]["radius"]});

    }

    // Create Ground Enemies
    for(auto tile: ls::findTiles(Color(ls::GENEMY))){
        auto enemy = makeEntity();
        enemy->setPosition(ls::getTilePosition(tile) +
                           Vector2f((float) JsonData::generalData["tileSize"] - (float) JsonData::enemies["gEnemy"]["radius"],
                                    (float) JsonData::generalData["tileSize"] - (float) JsonData::enemies["gEnemy"]["radius"]));
        //enemy->setPosition(Vector2f(ls::getWidth()*20/2, ls::getHeight()*20/2));
        enemy->addComponent<HurtComponent>();
        auto s = enemy->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(JsonData::enemies["gEnemy"]["radius"]);
        s->getShape().setFillColor(Color(ls::GENEMY));
        s->getShape().setOrigin(Vector2f(JsonData::enemies["gEnemy"]["radius"],
                                         JsonData::enemies["gEnemy"]["radius"]));
        if(JsonData::playerData["difficulty"] == 0) enemy->addComponent<EnemyAIComponent>();
        else enemy->addComponent<EnemyTargetedAIComponent>();
        enemies.push_back(enemy);
    }

    // Create Turrets
    for (auto tile: ls::findTiles(Color(ls::TENEMY))){
        auto turret = makeEntity();
        turret->setPosition(ls::getTilePosition(tile) +
                            Vector2f((float) JsonData::generalData["tileSize"] - (float) JsonData::enemies["tEnemy"]["radius"],
                                     (float) JsonData::generalData["tileSize"] - (float) JsonData::enemies["tEnemy"]["radius"] * 2));
        auto s = turret->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(JsonData::enemies["tEnemy"]["radius"], 3);
        s->getShape().setFillColor(Color(ls::TENEMY));
        s->getShape().setOrigin(Vector2f(JsonData::enemies["tEnemy"]["radius"],
                                         JsonData::enemies["tEnemy"]["radius"]));
        turret->addComponent<EnemyTurretComponent>();
        enemies.push_back(turret);
    }

    if((int) JsonData::playerData["difficulty"] == 1) {
        // Create Flying Enemies
        for (auto tile: ls::findTiles(Color(ls::FENEMY))) {
            auto enemy = makeEntity();
            enemy->setPosition(ls::getTilePosition(tile) +
                               Vector2f((float) JsonData::generalData["tileSize"] -
                                        (float) JsonData::enemies["fEnemy"]["radius"],
                                        (float) JsonData::generalData["tileSize"] -
                                        (float) JsonData::enemies["fEnemy"]["radius"]));
            enemy->addComponent<HurtComponent>();
            auto s = enemy->addComponent<ShapeComponent>();
            s->setShape<sf::CircleShape>(JsonData::enemies["fEnemy"]["radius"]);
            s->getShape().setFillColor(Color(ls::FENEMY));
            s->getShape().setOrigin(Vector2f(JsonData::enemies["fEnemy"]["radius"],
                                             JsonData::enemies["fEnemy"]["radius"]));
            enemy->addComponent<EnemyAStarComponent>();
            enemies.push_back(enemy);
        }
    }

    // Add physics colliders to level tiles.
    {
        auto walls = ls::findTiles(sf::Color(ls::WALL));
        for (auto w : walls) {
            auto pos = ls::getTilePosition(w);
            pos += Vector2f((float) JsonData::generalData["tileSize"] / 2,
                            (float) JsonData::generalData["tileSize"] / 2); //offset to center
            auto e = makeEntity();
            e->setPosition(pos);
            e->addComponent<PhysicsComponent>(false,
                                              Vector2f((float) JsonData::generalData["tileSize"],
                                                       (float) JsonData::generalData["tileSize"]));
        }
    }

    cout << " Scene 3 Load Done" << endl
         << "Player Health: " << player->get_components<EntityHealth>()[0]->getHealth() << endl;
    setLoaded(true);
}

void Level3Scene::UnLoad() {
    cout << "Scene 3 Unload" << endl;
    player.reset();
    ls::unload();
    Scene::UnLoad();
}

void Level3Scene::Update(const double& dt) {
    sf::View view(sf::FloatRect({0.f, 0.f},
                                {JsonData::generalData["viewWidth"],
                                 JsonData::generalData["viewHeight"]}));
    view.setCenter({player->getPosition().x, player->getPosition().y});
    Engine::GetWindow().setView(view);
    Scene::Update(dt);
    if (ls::getTileAt(player->getPosition()) == Color(ls::END) && !key->isAlive()) {
        JsonData::playerData["currentLevel"] = 1;
        JsonData::playerData["currentHealth"] =
                player->get_components<EntityHealth>()[0]->getHealth();
        JsonData::updatePlayerData();
        Engine::ChangeScene((Scene*)&menu);
        return;
    }
    if(!player->isAlive()){
        Engine::ChangeScene((Scene*)&level3);
        return;
    }
}

void Level3Scene::Render() {
    ls::render(Engine::GetWindow());
    Scene::Render();
}