#pragma once
#include <engine.h>
#include "../JsonData.h"

class EntityHealth: public Component {
protected:
    int _health;
    bool _cooldown;
    double _dtCounter;

public:
    void update(double dt) override;
    void render() override {};

    void adjustHealth(int amount);
    int getHealth() const;
    EntityHealth(Entity* p, int health);
    EntityHealth() = delete;
};