#pragma once
#include <ecm.h>

class KeyComponent: public Component {
protected:
    std::weak_ptr<Entity> _player;

public:
    void update(double dt) override;
    void render() override {};
    explicit KeyComponent(Entity* p);
    KeyComponent() = delete;
};