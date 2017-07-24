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

    this->addChild(_button);

    auto wheel = Sprite::createWithSpriteFrameName(Variables::WHEEL);

    wheel->runAction(RepeatForever::create(
            Sequence::create(
                    DelayTime::create(.5f),
                    RotateBy::create(1.f, 360.f),
                    NULL
            )));

    this->addChild(wheel);

    return true;
}

Size RichWheelButton::getBoundingBox() {
    return _button->getBoundingBox().size;
}
