//
// Created by igor on 05.10.16.
//

#include <GameEngine/Global/Variables.h>
#include "EquipmentScene.h"

USING_NS_CC;

bool EquipmentScene::init() {
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto stand = Sprite::createWithSpriteFrameName(Variables::STAND);
    stand->setPosition(Vec2(stand->getContentSize().width / 2, stand->getContentSize().height / 2));
    this->addChild(stand, 1);

    return true;
}

void EquipmentScene::onEnter() {
    Node::onEnter();
}

void EquipmentScene::onQuit() {

}
