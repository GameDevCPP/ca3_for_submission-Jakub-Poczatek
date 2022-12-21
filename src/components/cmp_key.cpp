#include "cmp_key.h"
#include <engine.h>

using namespace std;
using namespace sf;

void KeyComponent::update(double dt) {
    if (auto pl = _player.lock()) {
        if (length(pl->getPosition() - _parent->getPosition()) < 25.0) {
            _parent->setForDelete();
        }
    }
}

KeyComponent::KeyComponent(Entity* p)
        : Component(p), _player(_parent->scene->ents.find("player")[0]) {}