//
// Created by igor on 10.10.16.
//

#include <Scenes/PlayLayers/Battle.h>
#include <GameEngine/Global/WeaponSelector.h>
#include "UI.h"

USING_NS_CC;

UI::UI() {}

UI::~UI() {}

bool UI::init() {
    if (!cocos2d::LayerColor::init()) {
        return false;
    }

    return true;
}

void UI::initBattle(Size visibleSize, Hero *player) {
    auto b = Sprite::create("0.png");
    b->setScale(1);
    b->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(b);


    auto item1 = MenuItemFont::create("PAUSE", CC_CALLBACK_0(BattleScene::_pause, BattleScene::instance));

    auto menu = Menu::create(item1, nullptr);
    menu->alignItemsVertically();
    menu->setPosition(Vec2(visibleSize.width - 50, 25));
    this->addChild(menu);

    this->addChild(new WeaponSelector(player));
}

void UI::initDuel(Size visibleSize, Hero *player) {
    auto b = Sprite::create("1.png");
    b->setScale(1);
    b->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(b);


    auto item1 = MenuItemFont::create("PAUSE", CC_CALLBACK_0(BattleScene::_pause, BattleScene::instance));

    auto menu = Menu::create(item1, nullptr);
    menu->alignItemsVertically();
    menu->setPosition(Vec2(visibleSize.width - 50, 25));
    this->addChild(menu);
}

void UI::initApple(Size visibleSize, Hero *player) {
    auto b = Sprite::create("2.png");
    b->setScale(1);
    b->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(b);


    auto item1 = MenuItemFont::create("PAUSE", CC_CALLBACK_0(BattleScene::_pause, BattleScene::instance));

    auto menu = Menu::create(item1, nullptr);
    menu->alignItemsVertically();
    menu->setPosition(Vec2(visibleSize.width - 50, 25));
    this->addChild(menu);
}
