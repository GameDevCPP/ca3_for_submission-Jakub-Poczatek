#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_pickup.h"
#include "../components/cmp_entity_health.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> key;

void Level1Scene::Load() {
  cout << " Scene 1 Load" << endl;

  ls::loadLevelFile("../../res/levelFiles/level1.bmp", (float) JsonData::generalData["tileSize"]);
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
        auto playerHealth = player->addComponent<EntityHealth>(JsonData::playerData["currentHealth"]);
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

  //Simulate long loading times
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  cout << " Scene 1 Load Done" << endl
  << "Player Health: " << player->get_components<EntityHealth>()[0]->getHealth() << endl;
  setLoaded(true);
}

void Level1Scene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {
    // Set the view
    sf::View view(sf::FloatRect({0.f, 0.f},
                                {JsonData::generalData["viewWidth"],
                                 JsonData::generalData["viewHeight"]}));

    view.setCenter({player->getPosition().x, player->getPosition().y});
    Engine::GetWindow().setView(view);

    Scene::Update(dt);
    if (ls::getTileAt(player->getPosition()) == Color(ls::END) && !key->isAlive()) {
        JsonData::playerData["currentLevel"] = 2;
        JsonData::updatePlayerData();
        Engine::ChangeScene((Scene*)&level2);
        return;
    }
}

void Level1Scene::Render() {
    ls::render(Engine::GetWindow());
    Scene::Render();
}
