#include <GameEngine/Global/Variables.h>
#include <Localization/LocalizedStrings.h>
#include <Scenes/MenuLayers/Main/MainMenu.h>
#include <Scenes/MenuLayers/Main/Randomizer.h>
#include "GiftAlert.h"
#include "PopUp.h"

bool GiftAlert::init() {
    if(!Node::init()){
        return  false;
    }

    _board = Sprite::createWithSpriteFrameName(Variables::BOARD);
    this->addChild(_board, 2);

    _alertOn = Sprite::createWithSpriteFrameName(Variables::ALERT_ON);
    _alertOn->setPosition(Vec2(
            _board->getPosition().x,
            1.2f * _board->getBoundingBox().size.height

    ));
    _alertOn->setVisible(false);

    this->addChild(_alertOn, 1);

    _alertOff = Sprite::createWithSpriteFrameName(Variables::ALERT_OFF);
    _alertOff->setPosition(_alertOn->getPosition());
    this->addChild(_alertOff, 0);

    _def = cocos2d::UserDefault::getInstance();

    _onCreate();

    return true;
}

void GiftAlert::saveTime() {
    long time = 0;
    if(_clocks != nullptr){
        time = _clocks->getCurrent() - Variables::GIFT_INTERVAL + _clocks->timeLeft();
    }
    _def->setStringForKey("giftTime", StringUtils::toString(time));
}

void GiftAlert::touch() {
    switch(_state){
        case State::ON : {
            _switchState(State::OFF);
            MainScene::getInstance()->replaceMain(Randomizer::create());
            break;
        }

        case State::OFF : {
            _showPopUp();
            break;
        }
    }
}

void GiftAlert::_onCreate() {
    auto time = _def->getStringForKey("giftTime", "0");
    if(time != "0") {
        auto ts = std::atol(time.c_str());
        auto curr = Variables::getCurrentTime();
        auto diff = curr - ts;
        auto interval = Variables::GIFT_INTERVAL;
        if(diff < interval) {
            _switchState(State::ON);
        } else {
            _timeToWait = diff - interval;
            _switchState(State::OFF);
        }
    } else {
        _switchState(State::OFF);
    }
}

void GiftAlert::_switchState(GiftAlert::State state) {
    if(state == _state){
        return;
    }

    switch(state){
        case State::ON : {
            saveTime();
            _alertOn->runAction(RepeatForever::create(
                    Sequence::create(
                            CallFunc::create([&]() {
                                _alertOn->setVisible(true);
                            }),
                            DelayTime::create(.8f),
                            CallFunc::create([&]() {
                                _alertOn->setVisible(false);
                            }),
                            DelayTime::create(.8f),
                            NULL
                    )
            ));
            _board->removeAllChildren();
            auto tapMessage = Label::createWithTTF(LocalizedStrings::getInstance()->getString("GIFT"), Variables::FONT_NAME, Variables::FONT_SIZE());
            tapMessage->setPosition(Vec2(
                    _board->getContentSize().width / 2,
                    1.2f * _board->getContentSize().height / 2
            ));
            _board->addChild(tapMessage);
            break;
        }
        case State::OFF : {
            _reset();
            break;
        }
    }

    _state = state;
}

void GiftAlert::_reset() {
    _board->removeAllChildren();

    _alertOn->stopAllActions();
    _alertOn->setVisible(false);

    saveTime();

    if(_timeToWait){
        _clocks = Clocks::create(_timeToWait, [&](){
            _switchState(State::ON);
        }, 2);
        _timeToWait = 0;
    }
    else {
        _clocks = Clocks::create(Variables::getCurrentTime() + Variables::GIFT_INTERVAL, [&](){
            _switchState(State::ON);
        }, 2);
    }
    _clocks->setPosition(Vec2(
            _board->getContentSize().width / 2,
            1.2f * _board->getContentSize().height / 2
    ));
    _board->addChild(_clocks);
}

void GiftAlert::_showPopUp() {
    auto label = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("ERROR"),
                                               Variables::FONT_NAME,
                                               Variables::FONT_SIZE());
    label->setColor(cocos2d::Color3B::BLACK);
    auto popUp = MainMenuPopUp::create("", label);
    dynamic_cast<MainMenu*>(MainScene::getInstance()->getMain())->showPopUp(popUp);
}

Rect GiftAlert::getBoundingBox() const {
    auto box = _alertOn->getBoundingBox();
    auto min = Variables::translatePoint(Vec3(_alertOn->getBoundingBox().getMinX(), _alertOn->getBoundingBox().getMinY(), 0), _alertOn);
    auto max = Variables::translatePoint(Vec3(_alertOn->getBoundingBox().getMaxX(), _alertOn->getBoundingBox().getMaxY(), 0), _alertOn);
    Rect rect = Rect(min.x, min.y , 2.f * (max.x - min.x),  (max.y - min.y));
    return rect;
}
