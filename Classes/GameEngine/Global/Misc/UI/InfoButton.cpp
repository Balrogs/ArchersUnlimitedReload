#include <ui/UIButton.h>
#include <GameEngine/Global/Variables.h>
#include "InfoButton.h"

bool InfoButton::init(std::string text, std::string buttonPath) {
    if(!cocos2d::Node::init()){
        return  false;
    }

    auto button = cocos2d::ui::Button::create(buttonPath, buttonPath, buttonPath, cocos2d::ui::Widget::TextureResType::PLIST);

    button->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                switch (_state) {
                    case State::Showing : {
                        this->hide();
                        break;
                    }

                    case State::Idle : {
                        this->show();
                        break;
                    }
                }
                default:
                    break;
            }
        }
    });

    this->addChild(button, 1);

    _idlePos = cocos2d::Vec2(
            button->getPosition().x,
            button->getBoundingBox().getMaxY()
    );


    _cloud = cocos2d::Sprite::createWithSpriteFrameName(Variables::GREEN_BUTTON);
    _cloud->setPosition(cocos2d::Vec2(
            button->getPosition().x,
            button->getBoundingBox().getMaxY()
    ));
    this->addChild(_cloud, 2);

    _showingPos = cocos2d::Vec2(
            button->getPosition().x,
            button->getBoundingBox().getMaxY() + _cloud->getContentSize().height
    );

    _cloud->setScale(0);

    auto label = cocos2d::Label::createWithTTF(text, Variables::FONT_NAME, Variables::FONT_SIZE());
    label->setPosition(_cloud->getPosition());
    _cloud->addChild(label);

    hide();

    return true;
}

void InfoButton::_switchState(InfoButton::State state) {
    if(state == _state){
        return;
    }

    switch(state){
        case State::Showing : {
            _cloud->runAction(cocos2d::Spawn::create(
                    cocos2d::MoveTo::create(0.5f, _showingPos),
                    cocos2d::ScaleTo::create(0.5f, 1.0f),
                    NULL
            ));
            break;
        }

        case State::Idle : {
            _cloud->runAction(cocos2d::Spawn::create(
                    cocos2d::MoveTo::create(0.5f, _idlePos),
                    cocos2d::ScaleTo::create(0.5f, 0.0f),
                    NULL
            ));
            break;
        }
    }

    _state = state;
}

void InfoButton::show() {
    _switchState(State::Showing);
}

void InfoButton::hide() {
    _switchState(State::Idle);
}

InfoButtonTime *InfoButtonTime::create() {
    InfoButtonTime *ret = new(std::nothrow) InfoButtonTime();

    if (ret && ret->init("TIME LIMIT 50 SEC", Variables::FIND_BUTTON)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
