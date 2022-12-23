#pragma once
#include "cmp_pickup.h"
#include "cmp_entity_health.h"

class HealthPickup: public PickupComponent {
protected:
    std::weak_ptr<Entity> _player;

public:
    void update();
    HealthPickup(Entity* p);
    ~HealthPickup();
};

