#pragma once
#include <engine.h>

class EntityHealth: public Component {
protected:
    int _health;
    bool _cooldown;
    double _dtCounter;

public:
    void update(double dt) override;
    void render() override {};

    void adjustHealth(int amount);
    EntityHealth(Entity* p, int health);
    EntityHealth() = delete;
};