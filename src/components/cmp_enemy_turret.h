#pragma once
#include "cmp_physics.h"
#include "cmp_bullet.h"
#include <ecm.h>

class EnemyTurretComponent : public Component {
protected:
  void fire();
  float _firetime;
  std::vector<std::shared_ptr<Entity>> _bullets;
  int _bulletCounter;

public:
  void update(double dt) override;
  void render() override {}
  explicit EnemyTurretComponent(Entity* p);
  EnemyTurretComponent() = delete;
};
