#include <ui/UIButton.h>
#include <GameEngine/Global/Variables.h>
#include "InfoButton.h"

bool InfoButton::init(std::string text, std::string buttonPath) {
    if(!Node::init()){
        return  false;
    }

    auto button = ui::Button::create(buttonPath, buttonPath, buttonPath, ui::Widget::TextureResType::PLIST);

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

    _idlePos = Vec2(
            button->getPosition().x,
            button->getBoundingBox().getMaxY()
    );


    _cloud = Sprite::createWithSpriteFrameName(Variables::GREEN_BUTTON);
    _cloud->setPosition(Vec2(
            button->getPosition().x,
            button->getBoundingBox().getMaxY()
    ));
    this->addChild(_cloud, 2);

    _showingPos = Vec2(
            button->getPosition().x,
            button->getBoundingBox().getMaxY() + _cloud->getContentSize().height
    );

    _cloud->setScale(0);

    auto label = Label::createWithTTF(text, Variables::FONT_NAME, Variables::FONT_SIZE());
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
            _cloud->runAction(Spawn::create(
                    MoveTo::create(0.5f, _showingPos),
                    ScaleTo::create(0.5f, 1.0f),
                    NULL
            ));
            break;
        }

        case State::Idle : {
            _cloud->runAction(Spawn::create(
                    MoveTo::create(0.5f, _idlePos),
                    ScaleTo::create(0.5f, 0.0f),
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
