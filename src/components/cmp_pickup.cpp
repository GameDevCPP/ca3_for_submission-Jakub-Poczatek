#include "cmp_pickup.h"

using namespace std;
using namespace sf;

void PickupComponent::update(double dt) {
    if (auto pl = _player.lock()) {
        if (length(pl->getPosition() - _parent->getPosition()) < 25.0) {
            Audio::play(JsonData::audio["pickup"]);
            _parent->setForDelete();
        }
    }
}

PickupComponent::PickupComponent(Entity* p)
        : Component(p), _player(_parent->scene->ents.find("player")[0]) {}

std::weak_ptr<Entity> PickupComponent::getPlayer() {
    return _player.lock();
}