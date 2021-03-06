#include <Localization/LocalizedStrings.h>
#include <GameEngine/Global/Variables.h>
#include <ui/UIButton.h>
#include "Statistics.h"

Statistics *Statistics::create(int type) {
    Statistics *ret = new(std::nothrow) Statistics();
    if (!ret || !ret->init(type)) {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool Statistics::init(int type) {

    _view = Node::create();

    _type = type;
    _playerEnv = 0;
    _targetEnv = 0;
    _coinsGained = 0;
    _level = 0;
    _hp = 100;
    _timeSpent = 0;
    _victory = false;

    return true;
}

int Statistics::getLevel() {
    return _level;
}

void Statistics::increaseLevel(int level) {
    Statistics::_level = level;
}

int Statistics::getCoinsGained() {
    return _coinsGained;
}

void Statistics::increaseCoins(int diff) {
    Statistics::_coinsGained += diff;
}

long Statistics::getTimeSpent() {
    return _timeSpent;
}

void Statistics::setTime(long timeSpent) {
    Statistics::_timeSpent = timeSpent;
}

float Statistics::getHp() {
    return _hp;
}

void Statistics::setHp(float hp) {
    Statistics::_hp = hp;
}

int Statistics::getType() {
    return _type;
}

int Statistics::getPlayerEnvType() {
    return _playerEnv;
}

void Statistics::setPlayerEnvType(int type) {
    _playerEnv = type;
}

int Statistics::getTargetEnvType() {
    return _targetEnv;
}

void Statistics::setTargetEnvType(int type) {
    _targetEnv = type;
}

void Statistics::setWin() {
    _victory = true;
}

Node *Statistics::getView(Size contentSize) {
    auto parent = Node::create();

    parent->setContentSize(contentSize);

    if (_type != 1 && _type != 3) {
        auto string = LocalizedStrings::getInstance()->getString("YOU WIN");
        if (!_victory) {
            string = LocalizedStrings::getInstance()->getString("YOU LOOSE");
        }
        auto victory = Label::createWithTTF(
                string,
                Variables::FONT_NAME,
                Variables::FONT_SIZE());
        victory->setColor(Color3B::BLACK);

        victory->setPosition(Vec2(
                contentSize.width / 2,
                contentSize.height - victory->getContentSize().height
        ));
        parent->addChild(victory);
    }

    auto coins = Label::createWithTTF(
            StringUtils::format("%d", _coinsGained),
            Variables::FONT_NAME,
            Variables::FONT_SIZE());
    coins->setColor(Color3B::BLACK);
    coins->setPosition(contentSize.width / 2 - coins->getContentSize().width,
                       coins->getContentSize().height);
    parent->addChild(coins);

    auto coin = Sprite::createWithSpriteFrameName(Variables::COIN);
    coin->setPosition(Vec2(
            coins->getBoundingBox().getMaxX() + coin->getBoundingBox().size.width,
            coins->getPosition().y
    ));
    parent->addChild(coin);

//    //TODO replace with x2 button
//
//    auto doubleButton = cocos2d::ui::Button::create();
//    doubleButton->loadTextures(Variables::CHEST_BUTTON, Variables::CHEST_PRESSED_BUTTON,
//                              Variables::CHEST_BUTTON, ui::Widget::TextureResType::PLIST);
//
//    doubleButton->addTouchEventListener([&, coins](Ref *sender, ui::Widget::TouchEventType type) {
//        switch (type) {
//            case ui::Widget::TouchEventType::ENDED: {
//                //TODO show ad then double earned coins
//            }
//                break;
//            default:
//                break;
//        }
//    });
//    doubleButton->setPosition(Vec2(
//            coin->getBoundingBox().getMaxX() + 2 * doubleButton->getBoundingBox().size.width / 3,
//            coin->getPositionY()
//    ));
//
//    parent->addChild(doubleButton, 3);

    return parent;
}

bool Statistics::isVictory() {
    return _victory;
}

