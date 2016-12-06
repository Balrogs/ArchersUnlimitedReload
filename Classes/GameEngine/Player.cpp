#include <Scenes/PlayLayers/Battle.h>
#include <GameEngine/Global/Variables.h>

USING_NS_CC;

Player::Player() {

}

Player *Player::create(int id, int hp, std::string name) {
    Player *ret = new(std::nothrow) Player();
    if (ret && ret->init(id, hp, name)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;

}

Player *Player::create(int hp, std::string name) {
    auto id = BattleScene::instance->getStickmanCount();
    return Player::create(id, hp, name);
}

bool Player::init(int id, int hp, std::string name) {
    if (!Node::init()) {
        return false;
    }

    _id = id;
    _name = name;
    _shotsCount = 0;
    _hp = hp;

    auto size = cocos2d::Director::getInstance()->getVisibleSize();

    _name_view = cocos2d::Label::createWithTTF(_name.c_str(), Variables::FONT_NAME, 32.f, cocos2d::Size(size.width / 2, 0.f));

    _hp_view = cocos2d::Label::createWithTTF("", Variables::FONT_NAME, 32.f, cocos2d::Size(size.width / 2, 0.f));

    _shots_view = cocos2d::Label::createWithTTF("", Variables::FONT_NAME, 32.f, cocos2d::Size(size.width / 2, 0.f));

    _name_view->setPosition(size.width / 4, size.height - 50.f);
    _hp_view->setPosition(size.width / 4, size.height - 80.f);
    _shots_view->setPosition(size.width / 4, size.height - 120.f);
    _name_view->setColor(cocos2d::Color3B::BLACK);
    _hp_view->setColor(cocos2d::Color3B::BLACK);
    _shots_view->setColor(cocos2d::Color3B::BLACK);

    this->addChild(_name_view, 1, "name");
    this->addChild(_hp_view, 1, "hp");
    this->addChild(_shots_view, 1, "shotsCount");

    updateView();

    return true;
}

void Player::updateView() {

    _hp_view->setString(cocos2d::StringUtils::format("HP: %d", _hp));
    _shots_view->setString(cocos2d::StringUtils::format("%d", _shotsCount));
}


int Player::getId() {
    return _id;
}

std::string Player::getName() {
    return _name;
}


int Player::getShotsCount() {
    return _shotsCount;
}

void Player::addShotsCount() {
    _shotsCount += 1;
    updateView();
}


int Player::getHp() {
    return _hp;
}

void Player::setHp(int diff) {
    _hp -= diff;
    updateView();
}

void Player::setHAlignment(cocos2d::TextHAlignment alignment) {

    _name_view->setHorizontalAlignment(alignment);
    _hp_view->setHorizontalAlignment(alignment);
    _shots_view->setHorizontalAlignment(alignment);

}


