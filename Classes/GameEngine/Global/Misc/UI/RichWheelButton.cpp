#include <GameEngine/Global/Variables.h>
#include <Scenes/MenuLayers/Main/MainMenu.h>
#include <Scenes/MenuLayers/Multiplayer/Lobby.h>
#include <Scenes/MenuLayers/Main/Randomizer.h>
#include <Localization/LocalizedStrings.h>
#include "RichWheelButton.h"

bool RichWheelButton::init() {
    if(!Node::init()){
        return  false;
    }
    _enabled = false;

    _button = ui::Button::create(Variables::BLUE_EMPTY_BUTTON, Variables::BLUE_EMPTY_PRESSED_BUTTON, Variables::BLUE_EMPTY_BUTTON, ui::Widget::TextureResType::PLIST);

    _button->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                _checkEnabled();
                if(_enabled) {
                    MainScene::getInstance()->replaceMain(Randomizer::create());
                } else {
                    _showMessage();
                }
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
    _highlighting->setVisible(false);
    this->addChild(_highlighting, 0);

    _def = cocos2d::UserDefault::getInstance();

    _checkEnabled();

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

void RichWheelButton::_showMessage() {

}

void RichWheelButton::_checkEnabled() {
    _def->setStringForKey("wheelTime", StringUtils::toString(Variables::getCurrentTime() + 5000l));
    auto time = _def->getStringForKey("wheelTime", "0");
    auto ts = std::atol(time.c_str());
    auto curr = Variables::getCurrentTime();
    auto timeToCount = ts - curr;

    if(timeToCount > 0){
        _clocks = Clocks::create(ts, [&](){
            _highlight();
        });
        _clocks->setPosition(Vec2(
                _clocks->getContentSize().width,
                0
        ));
        this->addChild(_clocks);
    } else {
        _enabled = true;
        _highlight();
    }
}

bool RichWheelButtonCoins::init() {
    return RichWheelButton::init();
}

void RichWheelButtonCoins::_showMessage() {

    auto mainMenu = (MainMenu*) MainScene::getInstance()->getMain();

    Node * message = Node::create();

    auto label = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("WHEEL OF FORTUNE COSTS"),
                                               Variables::FONT_NAME,
                                               Variables::FONT_SIZE());
    label->setColor(cocos2d::Color3B::BLACK);
    label->setPosition(0, label->getBoundingBox().size.height);
    message->addChild(label);

    auto coins = Label::createWithTTF(
            StringUtils::format("%d", Variables::wheelCost()),
            Variables::FONT_NAME,
            Variables::FONT_SIZE());
    coins->setColor(Color3B::BLACK);
    coins->setPosition(0, - label->getBoundingBox().size.height);
    message->addChild(coins);

    auto coin = Sprite::createWithSpriteFrameName(Variables::COIN);
    coin->setPosition(Vec2(
            coins->getBoundingBox().getMaxX() + coin->getBoundingBox().size.width,
            coins->getPosition().y
    ));
    message->addChild(coin);

    auto popUp = MainMenuPopUp::create("", message, false);

    mainMenu->showPopUp(popUp);
}

void RichWheelButtonCoins::_checkEnabled() {
    auto coinsCount = _def->getIntegerForKey("COINS", 0);

    if(coinsCount > Variables::wheelCost()){
        _enabled = true;
        _highlight();
    }

}
