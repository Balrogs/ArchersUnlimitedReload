#include <GameEngine/Global/Variables.h>
#include "ContinueButton.h"
#include "ButtonWithPrice.h"

bool ContinueButton::init() {
    if(!Node::init()){
        return  false;
    }
    _enabled = false;

    _button = ui::Button::create(Variables::CONTINUE_BUTTON, Variables::CONTINUE_PRESSED_BUTTON, Variables::CONTINUE_BUTTON, ui::Widget::TextureResType::PLIST);

    _button->setEnabled(_enabled);

    this->addChild(_button, 1);

    _loading = Sprite::createWithSpriteFrameName(Variables::COIN);
    _loading->setPosition(Vec2(
            _button->getBoundingBox().getMaxX() + _loading->getContentSize().width / 2,
            _button->getBoundingBox().getMinY() + _loading->getContentSize().height / 2
    ));
    _loading->runAction(RepeatForever::create(RotateBy::create(1.f, 12.f)));
    _button->addChild(_loading, 2);

    return true;
}

Size ContinueButton::getBoundingBox() {
    return _button->getBoundingBox().size;
}

void ContinueButton::enable() {
    _enabled = true;

    _loading->removeFromParent();

    _button->setEnabled(_enabled);

    _price = Node::create();

    auto coins = Label::createWithTTF(
            StringUtils::format("%d", Variables::continueCost()),
            Variables::FONT_NAME,
            Variables::FONT_SIZE());
    coins->setColor(Color3B::WHITE);
    _price->addChild(coins);

    auto coin = Sprite::createWithSpriteFrameName(Variables::COIN);
    coin->setPosition(Vec2(
            coins->getBoundingBox().getMaxX() + coin->getBoundingBox().size.width,
            coins->getPosition().y
    ));
    _price->addChild(coin);

    _price->setPosition(Vec2(
            _button->getBoundingBox().getMaxX() + coins->getContentSize().width / 2,
            _button->getPosition().y
    ));
    _button->addChild(_price, 2);

    _price->runAction(RepeatForever::create(
                    Sequence::create(
                            ScaleTo::create(1.f, 1.f),
                            ScaleTo::create(1.f, 1.3f),
                            NULL
                    )
            ));
}

ui::Button *ContinueButton::button() {
    return _button;
}
