#include "RichEventButton.h"
#include <GameEngine/Global/Variables.h>
#include <Scenes/MenuLayers/Main/MainMenu.h>

bool RichEventButton::init() {
    if(!Node::init()){
        return  false;
    }


    auto button = ui::Button::create(Variables::FIND_BUTTON, Variables::FIND_BUTTON, Variables::FIND_BUTTON, ui::Widget::TextureResType::PLIST);

    button->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                if(_tryCount > 0){
                  decreaseTryCount();
                    // TODO start event
                } else {
                    // TODO show ads that start event
                }
                default:
                    break;
            }
        }
    });

    this->addChild(button);

    _tryLabel = Label::createWithTTF(StringUtils::format("%d",_tryCount), Variables::FONT_NAME, Variables::FONT_SIZE());
    _tryLabel->setPosition(Vec2(
            4 * _tryLabel->getContentSize().width,
            button->getBoundingBox().getMaxY()
    ));
    this->addChild(_tryLabel);


    _movieButton = Sprite::createWithSpriteFrameName(Variables::CLOSE_BUTTON_PATH);

    _scale = 0.75f * button->getBoundingBox().size.width / _movieButton->getBoundingBox().size.width;
    _movieButton->setScale(0);

    this->addChild(_movieButton);

    _def = cocos2d::UserDefault::getInstance();

    reset();

    _onCreate();

    return true;
}

void RichEventButton::start() {
    _switchState(State::Waiting);
}

void RichEventButton::reset() {
    _switchState(State::Idle);
}

void RichEventButton::_switchState(State state) {
    if(state == _state){
        return;
    }

    switch(state){
        case State::Waiting : {
            saveTime();
            if(_timeToWait){
                _clocks = Clocks::create(_timeToWait, [&](){
                    increaseTryCount();
                });
                _timeToWait = 0;
            }
            else {
                _clocks = Clocks::create([&](){
                    increaseTryCount();
                });
            }
            _clocks->setPosition(Vec2(
                    _clocks->getContentSize().width,
                    0
            ));
            this->addChild(_clocks);
            break;
        }

        case State::Idle : {
            if(_clocks != nullptr)
                _clocks->removeFromParent();
            break;
        }
    }

    _state = state;
}

void RichEventButton::_updateTryCount() {

    _def->setIntegerForKey("tryCount", _tryCount);

    _tryLabel->setString(StringUtils::format("%d",_tryCount));

    if(_tryCount < Variables::MAX_TRY_COUNT){
        start();
    }
    //TODO add actions
    if(_tryCount > 0){
        _movieButton->setScale(0);
    } else {
        _movieButton->setScale(1);
    }
}

void RichEventButton::increaseTryCount() {
    if(_tryCount + 1 <= Variables::MAX_TRY_COUNT) {
        _tryCount++;
        reset();
        _updateTryCount();
    }
}
void RichEventButton::decreaseTryCount() {
    if(_tryCount - 1 >= 0) {
        _tryCount--;
        _updateTryCount();
    }
}

void RichEventButton::_onCreate() {
    auto tryCount = _def->getIntegerForKey("tryCount", Variables::MAX_TRY_COUNT);
    if(tryCount != Variables::MAX_TRY_COUNT) {
        auto time = _def->getStringForKey("tryCountTime", "0");
        if(time != "0") {

            auto ts = std::stol(time);
            auto curr = Variables::getCurrentTime();
            auto diff = curr - ts;
            auto interval = Variables::RESET_INTERVAL;
            while (diff > interval && tryCount < Variables::MAX_TRY_COUNT) {
                diff -= interval;
                tryCount++;
            }

            if(tryCount < Variables::MAX_TRY_COUNT){
                _timeToWait = curr  + interval - diff;
                start();
            }
        }
    }

    _tryCount = tryCount;
    _updateTryCount();
}

void RichEventButton::saveTime() {
    long time = 0;
    if(_clocks != nullptr){
        time = _clocks->getCurrent() - Variables::RESET_INTERVAL + _clocks->timeLeft();
    }
    _def->setStringForKey("tryCountTime", StringUtils::toString(time));
}
