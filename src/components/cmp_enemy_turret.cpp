#include "cmp_enemy_turret.h"
#include "cmp_hurt_player.h"
#include "engine.h"
#include <SFML/Graphics/CircleShape.hpp>
using namespace std;
using namespace sf;

void EnemyTurretComponent::update(double dt) {
  _firetime -= dt;
  if (_firetime <= 0.f) {
    fire();
    _firetime = 1.f;
  }
  static float angle = 0.f;
  angle += 1.f * dt;

  _parent->setRotation(180.f + sin(angle) * 45.f);
}

void EnemyTurretComponent::fire() {
    //auto pComp = _bullets[_bulletCounter]->get_components<PhysicsComponent>()[0];
    auto bullet = _bullets[_bulletCounter];
    auto p = bullet->get_components<PhysicsComponent>()[0];
    p->teleport(_parent->getPosition());
    p->impulse(sf::rotate(Vector2f(0, 15.f), -_parent->getRotation()));

    _bulletCounter++;
    if(_bulletCounter > 1){
        _bulletCounter = 0;
    }
}

EnemyTurretComponent::EnemyTurretComponent(Entity* p)
    : Component(p), _firetime(2.f), _bulletCounter(0) {
    for (int i = 0; i < 2; i++){
        auto bullet = _parent->scene->makeEntity();
        bullet->setPosition(Vector2f(-1000, 1000));
        bullet->addComponent<HurtComponent>();
        bullet->addComponent<BulletComponent>();

        auto s = bullet->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(8.f);
        s->getShape().setFillColor(Color::Red);
        s->getShape().setOrigin(Vector2f(8.f, 8.f));

        auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(8.f, 8.f));
        p->setRestitution(.4f);
        p->setFriction(.005f);

        _bullets.push_back(bullet);
    }
}
