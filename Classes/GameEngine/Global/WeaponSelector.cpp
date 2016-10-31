//
// Created by igor on 08.09.16.
//

#include <ui/UIButton.h>
#include <Scenes/PlayLayers/Battle.h>
#include "WeaponSelector.h"

using namespace cocos2d::ui;
const float WeaponSelector::POSITION_X = 100.f;
const float WeaponSelector::POSITION_Y = 100.f;

WeaponSelector::WeaponSelector(Hero *hero) {
    _hero = hero;

    auto button = cocos2d::ui::Button::create("CloseNormal.png", "CloseSelected.png");
    button->setPosition(cocos2d::Vec2(POSITION_X, POSITION_Y));
    button->addTouchEventListener([&](cocos2d::Ref *sender, Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                _hero->switchWeapon();
                break;
            default:
                break;
        }
    });
    this->addChild(button);
}

WeaponSelector::~WeaponSelector() {

}
