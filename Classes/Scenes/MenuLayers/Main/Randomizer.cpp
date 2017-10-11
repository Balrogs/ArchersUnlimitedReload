#include <GameEngine/Objects/Environment/Gift.h>
#include "Randomizer.h"
#include "MainMenu.h"

bool Randomizer::init() {
    if (!Layer::init()) {
        return false;
    }

    _visibleSize = Director::getInstance()->getVisibleSize();

    this->pause();

    const auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Randomizer::_touchHandlerBegin, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(Randomizer::_touchHandlerEnd, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    _gift = Gift::create();
    this->setPosition(_visibleSize.width / 2, _visibleSize.width / 2);
    this->addChild(_gift, 1, "gift");

    _gift->enable();

    return true;
}

void Randomizer::onEnter() {
    Node::onEnter();
    MainScene::getInstance()->wait(true);
}


bool Randomizer::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    return _gift->checkTouch(touch->getLocation());
}

bool Randomizer::_touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if(_gift->checkTouch(touch->getLocation())){
        _gift->breakBox();
    }
    return true;
}
