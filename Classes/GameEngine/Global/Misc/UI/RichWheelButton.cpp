#include <GameEngine/Global/Variables.h>
#include <Scenes/MenuLayers/Main/MainMenu.h>
#include <Scenes/MenuLayers/Multiplayer/Lobby.h>
#include <Scenes/MenuLayers/Main/Randomizer.h>
#include "RichWheelButton.h"

bool RichWheelButton::init() {
    if(!Node::init()){
        return  false;
    }

    _button = ui::Button::create(Variables::BLUE_EMPTY_BUTTON, Variables::BLUE_EMPTY_PRESSED_BUTTON, Variables::BLUE_EMPTY_BUTTON, ui::Widget::TextureResType::PLIST);

    _button->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                MainScene::getInstance()->replaceMain(Randomizer::create());
            }
                break;
            default:
                break;
        }
    });

    this->addChild(_button, 1);

    _wheel = Sprite::createWithSpriteFrameName(Variables::WHEEL);
    this->addChild(_wheel, 2);

    _highlighting = Sprite::createWithSpriteFrameName(Variables::WHEEL);
    this->addChild(_highlighting, 0);

    _def = cocos2d::UserDefault::getInstance();
    _def->setStringForKey("wheelTime", StringUtils::toString(Variables::getCurrentTime() + 5000l));
    auto time = _def->getStringForKey("wheelTime", "0");
    auto ts = std::atol(time.c_str());
    auto curr = Variables::getCurrentTime();
    auto timeToCount = ts - curr;

    if(timeToCount > 0){
        _highlighting->setVisible(false);
        _clocks = Clocks::create(ts, [&](){
            _highlight();
        });
        _clocks->setPosition(Vec2(
                _clocks->getContentSize().width,
                0
        ));
        this->addChild(_clocks);
    } else {
        _highlight();
    }
    return true;
}

Size RichWheelButton::getBoundingBox() {
    return _button->getBoundingBox().size;
}

void RichWheelButton::_highlight() {
    if(_clocks != nullptr){
        _clocks->removeFromParent();
    }

    _wheel->runAction(RepeatForever::create(
            Sequence::create(
                    DelayTime::create(.5f),
                    RotateBy::create(1.f, 360.f),
                    NULL
            )));

    _highlighting->setVisible(true);

    _highlighting->runAction(RepeatForever::create(
            Spawn::create(
             Sequence::create(
                     ScaleTo::create(0.75f, 0.6f),
                     ScaleTo::create(0.75f, 1.2f),
                     NULL
             ),
             RotateBy::create(1.5f, -360.f),
             NULL
            )));
}
