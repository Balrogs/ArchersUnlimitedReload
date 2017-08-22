#include <GameEngine/Global/Variables.h>
#include "ContinueButton.h"

bool ContinueButton::init() {
    if(!Node::init()){
        return  false;
    }
    _enabled = false;

    _button = ui::Button::create(Variables::CONTINUE_BUTTON, Variables::CONTINUE_PRESSED_BUTTON, Variables::CONTINUE_BUTTON, ui::Widget::TextureResType::PLIST);

    _button->setEnabled(_enabled);

    this->addChild(_button, 1);

    _loading = Sprite::createWithSpriteFrameName(Variables::WHEEL);
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

    _ad = Sprite::createWithSpriteFrameName(Variables::WHEEL);
    _ad->setPosition(Vec2(
            _button->getBoundingBox().getMaxX() + _ad->getContentSize().width / 2,
            _button->getBoundingBox().getMinY() + _ad->getContentSize().height / 2
    ));
    _button->addChild(_ad, 2);

    _button->runAction(RepeatForever::create(
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
