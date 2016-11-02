//
// Created by igor on 08.09.16.
//

#include <ui/UIButton.h>
#include <Scenes/PlayLayers/Battle.h>
#include "WeaponSelector.h"

using namespace cocos2d::ui;


WeaponSelector::WeaponSelector(Hero *hero) {

    _hero = hero;

    cocos2d::Size visibleSize = BattleScene::instance->visibleSize;

    _SIZE = cocos2d::Size(visibleSize.width / 6, visibleSize.width / 6);

    float radius = (float) sqrt(_SIZE.height * _SIZE.height + _SIZE.width * _SIZE.width);

    _box = new cocos2d::Rect(
            cocos2d::Vec2(visibleSize.width - radius - _SIZE.width / 4, visibleSize.height - radius - _SIZE.height / 4),
            cocos2d::Size(_SIZE.width / 4 + radius, _SIZE.height / 4 + radius));

    int i = 0;

    auto list = _hero->getWeaponList();

    int middle = (int) std::ceil(list.size() / 2);

    for (auto weapon : list) {
        auto button = cocos2d::Sprite::createWithSpriteFrameName("icon.png");

        float scale = 1.f - abs(middle - i) / 10.f;

        double angle;

        double diff = 90 / (list.size() + 2);
        angle = 90 - diff * (i + 1);

        double x = radius * cos(dragonBones::ANGLE_TO_RADIAN * angle);
        double y = radius * sin(dragonBones::ANGLE_TO_RADIAN * angle);

        cocos2d::Vec2 pos = cocos2d::Vec2((float) (visibleSize.width - x), (float) (visibleSize.height - y));

        i++;

        button->setPosition(pos);
        button->setScale(scale);
        this->addChild(button);
    }

    const auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(WeaponSelector::_touchHandlerBegin, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(WeaponSelector::_touchHandlerEnd, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

WeaponSelector::~WeaponSelector() {

}

bool WeaponSelector::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    return true;
}

bool WeaponSelector::_touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) {
    const auto start = touch->getStartLocation();
    if (!_box->containsPoint(start)) {
        return false;
    }
    const auto curr = touch->getLocation();
    float x = start.x - curr.x > 0 ? 0 : 1;
    float y = start.y - curr.y > 0 ? 0 : 1;
    if (y) {
        _goNext(1);
    } else {
        _goNext(-1);
    }
    return true;
}

void WeaponSelector::_goNext(int dest) {

    auto children = this->getChildren();

    int tmp_id;

    int i = 0;
    if (dest < 0) {
        i = (int) children.size() - 1;
        tmp_id = (int) (children.size() - 1);
    } else {
        tmp_id = 0;
    }
    cocos2d::Node *tmp_child = children.at(tmp_id);
    for (; i < children.size() && i >= 0; i += dest) {
        cocos2d::Action *action;
        int id = i + dest;
        cocos2d::Node *child;
        float delay = 0.1f;
        if (id < 0 || id > children.size() - 1) {
            child = tmp_child;
            id = tmp_id;
        } else {
            child = children.at(id);
        }
        action = cocos2d::Spawn::createWithTwoActions(
                cocos2d::MoveTo::create(delay, child->getPosition()),
                cocos2d::ScaleTo::create(delay, child->getScale())
        );
        children.at(i)->runAction(action);
    }
    _hero->switchWeapon(dest);
}
