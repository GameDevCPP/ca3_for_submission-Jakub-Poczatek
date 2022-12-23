#include "cmp_health_pickup.h"

HealthPickup::HealthPickup(Entity* p)
    : PickupComponent(p), _player(PickupComponent::getPlayer()) {}

HealthPickup::~HealthPickup() noexcept {
    if(auto pl = _player.lock())
        pl->get_components<EntityHealth>()[0]->adjustHealth(+1);
}