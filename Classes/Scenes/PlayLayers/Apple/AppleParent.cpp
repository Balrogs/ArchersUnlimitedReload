#include <GameEngine/Objects/Environment/Apple.h>
#include <Scenes/PlayLayers/Battle.h>
#include "AppleParent.h"

void AppleParent::setHit() {
    _isTargetHitted = true;
}

void AppleParent::setAppleHit() {
    _isAppleHitted = true;
    completeShot();
}

void AppleParent::removeTarget(cocos2d::Node *target) {
    if (!_targets.empty())
        _targets.erase(std::find(_targets.begin(), _targets.end(), target));
}

void AppleParent::completeShot() {
    _completedShots++;
}

void AppleParent::addApple(Body *target) {
    auto apple = new Apple(target->getPositionX(), target->getGlobalHeight("head"), target->getPlayer()->getId());
    _targets.push_back(apple);
}
