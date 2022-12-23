#pragma once
#include <ecm.h>
#include <engine.h>

class PickupComponent: public Component {
protected:
    std::weak_ptr<Entity> _player;

public:
    void update(double dt) override;
    void render() override {};

    std::weak_ptr<Entity> getPlayer();
    explicit PickupComponent(Entity* p);
    PickupComponent() = delete;
};