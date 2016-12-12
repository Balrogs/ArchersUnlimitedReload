//
// Created by igor on 05.10.16.
//

#include <GameEngine/Global/Variables.h>
#include "EquipmentScene.h"
#include "MainMenu.h"

USING_NS_CC;

bool EquipmentScene::init() {
    if (!Layer::init()) {
        return false;
    }

    _visibleSize = Director::getInstance()->getVisibleSize();

    auto stand = Sprite::createWithSpriteFrameName(Variables::STAND);
    stand->setPosition(Vec2(stand->getContentSize().width / 2, stand->getContentSize().height / 2));
    this->addChild(stand, 1);

    _hero = new HeroPreview();
    _hero->setScale(1.5f);
    _hero->setPosition(stand->getContentSize().width / 2 , stand->getContentSize().height / 2);
    this->addChild(_hero, 2);

    return true;
}

void EquipmentScene::onEnter() {
    Layer::onEnter();
}

void EquipmentScene::onQuit() {
    this->getParent()->addChild(MainMenu::create(), 4);
    this->removeFromParent();
}
