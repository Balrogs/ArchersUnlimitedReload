//s54
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


    _keyboardListener = cocos2d::EventListenerKeyboard::create();
    _keyboardListener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
        switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_BREAK:
            case EventKeyboard::KeyCode::KEY_ESCAPE:
            case EventKeyboard::KeyCode::KEY_BACKSPACE: {
              onQuit();
            }
                break;
            default:
                break;
        }
    };

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    auto stand = Sprite::createWithSpriteFrameName(Variables::STAND);
    stand->setPosition(Vec2(stand->getContentSize().width / 2, stand->getContentSize().height / 2));
    this->addChild(stand, 1);

    auto hero = new HeroPreview();
    hero->setScale(1.5f);
    hero->setPosition(stand->getContentSize().width / 2, stand->getContentSize().height / 2);
    this->addChild(hero, 2);

    _controls = UIControls::create(hero);
    _controls->setPosition(Vec2(
            0,
            0
                           )
    );
    this->addChild(_controls, 2);

    this->pause();

    return true;
}

void EquipmentScene::onEnter() {
    Layer::onEnter();

    //TODO zoom in

    _controls->setVisible(true);
}

void EquipmentScene::onQuit() {

    //TODO zoom out

    this->pause();
    _controls->setVisible(false);
    MainScene::getInstance()->popAndReplace();
}

cocos2d::Vec2 EquipmentScene::getButtonPosition() {
    return Vec2(_controls->getHero()->getPosition().x, _controls->getHero()->getGlobalHeight("Head"));
}

EquipmentScene::UIControls *EquipmentScene::UIControls::create(HeroPreview *hero) {
    UIControls *ret = new(std::nothrow) UIControls();
    if (ret && ret->init(hero)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentScene::UIControls::init(HeroPreview *hero) {
    if (!Node::init()){
        return false;
    }

    _hero = hero;

    return true;
}

HeroPreview *EquipmentScene::UIControls::getHero() {
    return _hero;
}

EquipmentScene::Selector *EquipmentScene::Selector::create(EquipmentScene::Alignment alignment) {
    Selector *ret = new(std::nothrow) Selector();
    if (ret && ret->init(alignment)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentScene::Selector::init(EquipmentScene::Alignment alignment) {
    if (!Node::init()){
        return false;
    }

    switch(alignment){
        case Alignment::Horizontal: {
            break;
        }
        case Alignment::Vertical: {
            break;
        }
    }

    return true;
}

EquipmentScene::Item *EquipmentScene::Item::create(Node* view, int ind, float value, bool isAvailable = true, BuyType type = BuyType::Money) {
    Item *ret = new(std::nothrow) Item();
    if (ret && ret->init(view, ind, isAvailable, type, value)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentScene::Item::init(Node* view, int ind, bool isAvailable, BuyType type, float value) {
    if (!Sprite::init()){
        return false;
    }

    return true;
}
