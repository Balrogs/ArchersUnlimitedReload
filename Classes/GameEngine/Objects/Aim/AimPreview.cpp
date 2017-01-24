//
// Created by igor on 20.09.16.
//

#include "AimPreview.h"
#include "Scenes/PlayLayers/Battle.h"

USING_NS_CC;

AimPreview::AimPreview(float scale, const cocos2d::Vec2 &position) {
    auto circle = DrawNode::create();
    circle->drawSolidCircle(Vec2(0, 0), 5.f, 10, 10, Color4F::BLACK);
    this->addChild(circle);
    this->setPosition(position);
    this->setScale(scale * BattleScene::getInstance()->getGlobalScale());
}

AimPreview::~AimPreview() {
}

void AimPreview::update(float scale, const cocos2d::Vec2 &position) {
    this->setPosition(position);
    this->setScale(scale * BattleScene::getInstance()->getGlobalScale());
}

void AimPreview::remove() {
    this->removeFromParent();
}