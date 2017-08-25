#include <GameEngine/Global/Variables.h>
#include "HitInfo.h"


bool HitInfo::init(Vec2 pos) {
    if (!Node::init()) {
        return false;
    }
    this->setPosition(Vec2(pos.x, pos.y + 50.f));
    this->runAction(Sequence::create(
            MoveBy::create(1.5f, Vec2(0.f, 40.f)),
            DelayTime::create(.5f),
            RemoveSelf::create(),
            NULL));

    return true;
}

DamageInfo *DamageInfo::create(Vec2 pos, int count) {
    DamageInfo *ret = new(std::nothrow) DamageInfo();
    if (ret && ret->init(pos, count)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DamageInfo::init(Vec2 pos, int count) {
    if (!HitInfo::init(pos)) {
        return false;
    }

    auto damage = Label::createWithTTF(
            StringUtils::format("%d", count),
            Variables::FONT_NAME,
            Variables::FONT_SIZE_2());

    damage->setColor(Color3B::ORANGE);
    damage->setPosition(Vec2::ZERO);

    this->addChild(damage);

    return true;
}

CoinInfo *CoinInfo::create(Vec2 pos, int count) {
    CoinInfo *ret = new(std::nothrow) CoinInfo();
    if (ret && ret->init(pos, count)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CoinInfo::init(Vec2 pos, int count) {
    if (!HitInfo::init(pos)) {
        return false;
    }

    auto coins = Label::createWithTTF(
            StringUtils::format("%d", count),
            Variables::FONT_NAME,
            Variables::FONT_SIZE_2());

    coins->setColor(Color3B::ORANGE);
    coins->setPosition(Vec2::ZERO);

    this->addChild(coins);

    auto coin = Sprite::createWithSpriteFrameName(Variables::COIN);

    coin->setPosition(Vec2(
            coins->getBoundingBox().getMaxX() + coin->getBoundingBox().size.width,
            coins->getPosition().y
    ));
    this->addChild(coin);

    return true;
}
