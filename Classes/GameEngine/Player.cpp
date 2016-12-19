#include <Scenes/PlayLayers/Battle.h>
#include <GameEngine/Global/Variables.h>

USING_NS_CC;

Player *Player::create(int id, int hp, std::string name) {
    Player *ret = new(std::nothrow) Player();
    if (ret && ret->init(id, hp, name)) {
        ret->retain();
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
                                               cocos2d::Size(size.width / 2 - 70.f, 0.f));
    _hp_view = HPBar::create(cocos2d::Size(size.width / 2 - 70.f, 50.f));
    _hp_view->setContentSize(cocos2d::Size(size.width / 2 - 70.f, 50.f));

    _shots_view = cocos2d::Label::createWithTTF(cocos2d::StringUtils::format("%d", _shotsCount), Variables::FONT_NAME,
                                                32.f, cocos2d::Size(size.width / 2 - 70.f, 0.f));

    _name_view->setAnchorPoint(Vec2::ZERO);
    _hp_view->setAnchorPoint(Vec2::ZERO);
    _shots_view->setAnchorPoint(Vec2::ZERO);

    _name_view->setPosition(0, size.height - 40.f);
    _hp_view->setPosition(0, size.height - 100.f);
    _shots_view->setPosition(0, size.height - 150.f);


    setHAlignment(TextHAlignment::LEFT);

    _name_view->setColor(cocos2d::Color3B::BLACK);
    _shots_view->setColor(cocos2d::Color3B::BLACK);

    this->addChild(_name_view, 1, "name");
    this->addChild(_hp_view, 1, "hp");
    this->addChild(_shots_view, 1, "shotsCount");

    updateView();

    return true;
}

void Player::updateView() {
    if (this->getParent() != nullptr)
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

void Player::nullShotsCount() {
    _shotsCount = 0;
    updateView();
}


HPBar *HPBar::create(cocos2d::Size size) {
    HPBar *ret = new(std::nothrow) HPBar();
    if (ret && ret->init(size)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool HPBar::init(cocos2d::Size size) {
    if (!Node::init())
        return false;

    _prevSize = size;
    _size = size;
    _hp = 100;
    _alignment = true;
    _state = NONE;
    setHp(_hp);

    return true;
}

void HPBar::setHp(float hp) {

    _hp = hp;
    auto percents = hp / 100.f;

    if (percents < 0.3f) {
        changeState(DANGER);
    } else if (percents < 0.6f) {
        changeState(NORMAL);
    } else if (percents >= 0.6f) {
        changeState(FULL);
    }

    float new_Size = percents * _size.width;

    float x = new_Size / _spriteSize.width;
    float y = _size.height / _spriteSize.height;

    _prevSize = _bar->getContentSize();

    if (!_alignment) {
        _bar->runAction(
                Spawn::createWithTwoActions(
                        MoveTo::create(0.2f, Vec2(_size.width - new_Size, 0)),
                        ScaleTo::create(0.2f, x, y)));
    } else {
        _bar->runAction(ScaleTo::create(0.2f, x, y));
    }
}

void HPBar::setAlignment(bool isLeft) {
    _alignment = isLeft;
    setHp(_hp);
}

void HPBar::changeState(HPState state) {
    if (_state == state) {
        return;
    }

    this->removeAllChildren();
    _state = state;

    switch (state) {
        case FULL: {
            _bar = Sprite::createWithSpriteFrameName("green_normal.png");
            break;
        }
        case NORMAL: {
            _bar = Sprite::createWithSpriteFrameName("yellow_normal.png");
            break;
        }
        case DANGER: {
            _bar = Sprite::createWithSpriteFrameName("red_normal.png");
            break;
        }
        default: {
            return;
        }
    }

    _bar->setAnchorPoint(Vec2(0, 0));
    _spriteSize = _bar->getContentSize();

    float x = _prevSize.width / _spriteSize.width;
    float y = _prevSize.height / _spriteSize.height;

    _bar->setScale(x, y);

    this->addChild(_bar);
}

PlayerWithCoins *PlayerWithCoins::create(int id, int hp, std::string name, int coins) {
    PlayerWithCoins *ret = new(std::nothrow) PlayerWithCoins();
    if (ret && ret->init(id, hp, name, coins)) {
        ret->retain();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;

}

PlayerWithCoins *PlayerWithCoins::create(int hp, std::string name) {
    auto id = BattleScene::instance->getStickmanCount();
    auto coins = 0;
    return PlayerWithCoins::create(id, hp, name, coins);
}

bool PlayerWithCoins::init(int id, int hp, std::string name, int coins) {
    if (!Player::init(id, hp, name)) {
        return false;
    }

    _hp_view->setVisible(false);

    auto size = cocos2d::Director::getInstance()->getVisibleSize();

    _coinsView = Node::create();
    _coinsView->setScale(0.5f);
    _coinsView->setPosition(Vec2(_coinsView->getBoundingBox().size.width / 2,
                                 size.height - _coinsView->getBoundingBox().size.height / 2 - 95.f));

    auto coin = Sprite::createWithSpriteFrameName(Variables::COIN);

    coin->setPosition(coin->getBoundingBox().size.width / 2, coin->getBoundingBox().size.height / 2);

    _coinsView->addChild(coin, 2, "coin");

    auto coinsCount = cocos2d::Label::createWithTTF(StringUtils::toString(coins), Variables::FONT_NAME,
                                                    Variables::H_FONT_SIZE);
    coinsCount->setColor(Color3B::BLACK);

    coinsCount->setPosition(coin->getBoundingBox().size.width + 10.f + coinsCount->getBoundingBox().size.width / 2,
                            coin->getBoundingBox().size.height / 2);

    _coinsView->addChild(coinsCount, 2, "count");

    this->addChild(_coinsView, 1, "coins_view");

    return true;
}

void PlayerWithCoins::setHAlignment(cocos2d::TextHAlignment alignment) {
    Player::setHAlignment(alignment);
}

void PlayerWithCoins::addGainedCoins(int newCount) {
    Node *coin = _coinsView->getChildByName("count");
    Label *count = (Label *) _coinsView->getChildByName("count");
    count->setString(StringUtils::toString(newCount));
    count->setPosition(coin->getBoundingBox().size.width + 10.f,
                       count->getPosition().y);
}
