#include <Scenes/PlayLayers/Battle.h>

int Player::getId() const {
    return _id;
}

const char *Player::getName() const {
    return _name;
}



int Player::getShotsCount() const {
    return _shotsCount;
}

void Player::addShotsCount() {
    _shotsCount += 1;
    updateView();
}


Player::Player(int hp, const char *name) {

    _id = BattleScene::instance->getStickmanCount();
    _name = name;
    _shotsCount = 0;
    _hp = hp;
    _view = cocos2d::Node::create();

    auto size = BattleScene::instance->visibleSize;

    _name_view = cocos2d::Label::createWithTTF("", "arial.ttf", 32.f, cocos2d::Size(size.width / 2 , 0.f));

    _hp_view = cocos2d::Label::createWithTTF("", "arial.ttf", 32.f, cocos2d::Size(size.width / 2 , 0.f));

    _shots_view = cocos2d::Label::createWithTTF("", "arial.ttf", 32.f, cocos2d::Size(size.width / 2 , 0.f));

    updateView();

    _name_view->setPosition(size.width / 4, size.height - 50.f);
    _hp_view->setPosition(size.width / 4, size.height - 80.f);
    _shots_view->setPosition(size.width / 4, size.height - 120.f);
    _name_view->setColor(cocos2d::Color3B::BLACK);
    _hp_view->setColor(cocos2d::Color3B::BLACK);
    _shots_view->setColor(cocos2d::Color3B::BLACK);

    _view->addChild(_name_view, 1, "name");
    _view->addChild(_hp_view, 1, "hp");
    _view->addChild(_shots_view, 1, "shotsCount");
}

void Player::updateView() {

    _name_view->setString(cocos2d::StringUtils::format("%s", _name));
    _hp_view->setString(cocos2d::StringUtils::format("HP: %d", _hp));
    _shots_view->setString(cocos2d::StringUtils::format("%d", _shotsCount));
}

cocos2d::Node *Player::getView() {
    return _view;
}

int Player::getHp() const {
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
