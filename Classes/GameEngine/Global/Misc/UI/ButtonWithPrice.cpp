#include <ui/UIButton.h>
#include <GameEngine/Global/Variables.h>
#include "ButtonWithPrice.h"

ButtonWithPrice *ButtonWithPrice::create(int cost) {
    ButtonWithPrice *ret = new(std::nothrow) ButtonWithPrice();
    if (ret && ret->init(cost)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ButtonWithPrice::init(int cost) {
    if(!Node::init()){
        return false;
    }

    _cost = cost;

    _def = cocos2d::UserDefault::getInstance();

    _coins_bar = cocos2d::ui::Button::create();
    _coins_bar->loadTextureNormal(Variables::COIN_BAR, cocos2d::ui::Widget::TextureResType::PLIST);

    _coins_bar->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                if(_checkEnable()){
                    _callback();
                }
            }
                break;
            default:
                break;
        }
    });

    _coins_bar->setScale(0.5f);


    auto count = cocos2d::Label::createWithTTF(StringUtils::toString(cost), Variables::FONT_NAME,
                                                Variables::FONT_SIZE(), Size(3 * _coins_bar->getContentSize().width / 5, Variables::FONT_SIZE()));
    count->setHorizontalAlignment(TextHAlignment::RIGHT);
    count->setColor(Color3B::WHITE);
    count->setAnchorPoint(Vec2(0, 0));
    count->setPosition(15.f, _coins_bar->getContentSize().height / 2 - Variables::FONT_SIZE() / 2);

    _coins_bar->addChild(count, 5);

    this->addChild(_coins_bar);
    _contentSize = _coins_bar->getContentSize();
    return true;
}

void ButtonWithPrice::addAction(std::function<void()> callback) {
    _callback = callback;
}

bool ButtonWithPrice::_checkEnable() {
   return  _callback != nullptr && _def->getIntegerForKey("COINS", 0) >= _cost;
}

void ButtonWithPrice::setEnable(bool enable) {
    _coins_bar->setEnabled(enable);
}

