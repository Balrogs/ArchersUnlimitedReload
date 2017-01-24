#include <GameEngine/Objects/Environment/Apple.h>
#include <Scenes/PlayLayers/Battle.h>
#include "AppleParent.h"

void AppleParent::setHit() {
    _isTargetHitted = true;
}

void AppleParent::setAppleHit() {
    completeShot();
    _isAppleHitted = true;
}

void AppleParent::removeTarget(cocos2d::Node *target) {
    if (!_targets.empty())
        _targets.erase(std::find(_targets.begin(), _targets.end(), target));
}

void AppleParent::completeShot() {
    _completedShots++;
}
