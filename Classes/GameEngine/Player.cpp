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

    _name_view = cocos2d::Label::createWithTTF(_name.c_str(), Variables::FONT_NAME, 32.f,
                                               cocos2d::Size(size.width / 2, 0.f));
    _hp_view = HPBar::create();
    _hp_view->setContentSize(cocos2d::Size(size.width / 2 - 100.f, 50.f));

    _shots_view = cocos2d::Label::createWithTTF("", Variables::FONT_NAME, 32.f, cocos2d::Size(size.width / 2, 0.f));

    _name_view->setPosition(size.width / 4, size.height - 40.f);
    _hp_view->setPosition(15.f, size.height - 120.f);
    _shots_view->setPosition(size.width / 4, size.height - 150.f);

    _name_view->setColor(cocos2d::Color3B::BLACK);
    _shots_view->setColor(cocos2d::Color3B::BLACK);

    this->addChild(_name_view, 1, "name");
    this->addChild(_hp_view, 1, "hp");
    this->addChild(_shots_view, 1, "shotsCount");

    updateView();

    return true;
}

void Player::updateView() {

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
    if (_hp > 0) {
        _hp_view->setHp(_hp);
    }
}

void Player::setHAlignment(cocos2d::TextHAlignment alignment) {

    _name_view->setHorizontalAlignment(alignment);
    _shots_view->setHorizontalAlignment(alignment);
    _hp_view->setAlignment(alignment != cocos2d::TextHAlignment::RIGHT);

}


HPBar *HPBar::create() {
    HPBar *ret = new(std::nothrow) HPBar();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool HPBar::init() {
    if (!Node::init())
        return false;
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _size = Size(visibleSize.width / 2 - 100.f, 50.f);

    _bar = Sprite::createWithSpriteFrameName("green_normal.png");
    _bar->setAnchorPoint(Vec2(0, 0));
    _bar->setPosition(Vec2(0, 0));

    auto oWidth = _bar->getContentSize().width;
    auto oHeight = _bar->getContentSize().height;
    _scale = _size.width / oWidth;
    _bar->setScale(_scale, _size.height / oHeight);
    _state = HPState::FULL;
    _hp = 100;
    _alignment = true;
    this->addChild(_bar);

    return true;
}

void HPBar::setHp(float hp) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    _hp = hp;
    auto percents = hp / 100.f;
    CCLOG("HP %f per %f", hp, percents);
    if (percents < 0.3f) {
        if (_state != HPState::DANGER) {
            this->removeAllChildren();
            _state = HPState::DANGER;
            _bar = Sprite::createWithSpriteFrameName("red_normal.png");
            _bar->setAnchorPoint(Vec2(0, 0));
            this->addChild(_bar);
        }
    } else if (percents < 0.6f && _state != HPState::NORMAL) {
        if (_state != HPState::NORMAL) {
            this->removeAllChildren();
            _state = HPState::NORMAL;
            _bar = Sprite::createWithSpriteFrameName("yellow_normal.png");
            _bar->setAnchorPoint(Vec2(0, 0));
            this->addChild(_bar);
        }
    } else if (percents >= 0.6f && _state != HPState::FULL) {
        if (_state != HPState::FULL) {
            this->removeAllChildren();
            _state = HPState::FULL;
            _bar = Sprite::createWithSpriteFrameName("green_normal.png");
            _bar->setAnchorPoint(Vec2(0, 0));
            this->addChild(_bar);
        }
    }

    auto oWidth = _size.width;
    auto oHeight = _bar->getContentSize().height;

    _size = Size(percents * _scale * visibleSize.width / 2 - 100.f, 50.f);
    float x = _size.width / oWidth;
    float y = _size.height / oHeight;

    if (!_alignment) {
        CCLOG("MOVETO x %f", oWidth - _size.width);
        _bar->runAction(
                Spawn::createWithTwoActions(MoveTo::create(0.2f, Vec2(oWidth - _size.width, 0)),
                                            ScaleTo::create(0.2f, x, y)));
    } else {
        _bar->runAction(ScaleTo::create(0.2f, x, y));
    }
}

void HPBar::setAlignment(bool isLeft) {
    _alignment = isLeft;
    setHp(_hp);
}
