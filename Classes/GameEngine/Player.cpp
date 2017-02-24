#include <Scenes/PlayLayers/Battle.h>
#include <GameEngine/Global/Variables.h>

USING_NS_CC;



bool Player::init(int id, std::string name) {
    if (!Node::init()) {
        return false;
    }

    _id = id;
    _name = name;

    _visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

    return true;
}


int Player::getId() {
    return _id;
}

std::string Player::getName() {
    return _name;
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

    _prevSize = Size(0.f, 0.f);
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

    _prevSize = Size(_bar->getScaleX(), _bar->getScaleY());

    float new_Size = percents * _size.width;

    float x = new_Size / _spriteSize.width;
    float y = _size.height / _spriteSize.height;

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
    if (_prevSize.height == 0 || _prevSize.width == 0) {
        _bar->setScale(_size.width / _spriteSize.width, _size.height / _spriteSize.height);
    } else {
        _bar->setScale(_prevSize.width, _prevSize.height);
    }

    if(!_alignment){
        _bar->setPosition(Vec2(_size.width - _bar->getContentSize().width, 0));
    }

    this->addChild(_bar);
}

PlayerDuel *PlayerDuel::create(int id, std::string name, int hp, int shotsCount) {
    PlayerDuel *ret = new(std::nothrow) PlayerDuel();
    if (ret && ret->init(id, name, hp, shotsCount)) {
        ret->retain();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool PlayerDuel::init(int id, std::string name, int hp, int shotsCount) {
    if (!Player::init(id, name)) {
        return false;
    }

    _hp = hp;
    _shotsCount = shotsCount;

    _nameView = cocos2d::Label::createWithTTF(_name.c_str(), Variables::FONT_NAME, 32.f,
                                               cocos2d::Size(_visibleSize.width / 2 - 70.f, 0.f));
    _hpView = HPBar::create(cocos2d::Size(_visibleSize.width / 2 - 70.f, 50.f));
    _hpView->setContentSize(cocos2d::Size(_visibleSize.width / 2 - 70.f, 50.f));

    _shotsView = cocos2d::Label::createWithTTF(cocos2d::StringUtils::format("%d", _shotsCount), Variables::FONT_NAME,
                                                32.f, cocos2d::Size(_visibleSize.width / 2 - 70.f, 0.f));

    _nameView->setAnchorPoint(Vec2::ZERO);
    _hpView->setAnchorPoint(Vec2::ZERO);
    _shotsView->setAnchorPoint(Vec2::ZERO);

    _nameView->setPosition(0, _visibleSize.height - 40.f);
    _hpView->setPosition(0, _visibleSize.height - 100.f);
    _shotsView->setPosition(0, _visibleSize.height - 150.f);


    setHAlignment(TextHAlignment::LEFT);

    _nameView->setColor(cocos2d::Color3B::BLACK);
    _shotsView->setColor(cocos2d::Color3B::BLACK);

    this->addChild(_nameView, 1, "name");
    this->addChild(_hpView, 1, "hp");
    this->addChild(_shotsView, 1, "shotsCount");

    return true;
}

void PlayerDuel::setHAlignment(cocos2d::TextHAlignment alignment) {
    _nameView->setHorizontalAlignment(alignment);
    _shotsView->setHorizontalAlignment(alignment);
    _hpView->setAlignment(alignment != cocos2d::TextHAlignment::RIGHT);
}

int PlayerDuel::getShotsCount() {
    return _shotsCount;
}

void PlayerDuel::addShotsCount() {
    _shotsCount += 1;
    _updateView();
}

void PlayerDuel::nullShotsCount() {
    _shotsCount = 0;
    _updateView();
}

int PlayerDuel::getHp() {
    return _hp;
}

void PlayerDuel::setHp(int diff) {
    _hp -= diff;
    if (_hp > 0) {
        _hpView->setHp(_hp);
    }
}

void PlayerDuel::_updateView() {
    //TODO
}

std::string PlayerDuel::getPlayerView() {
    //TODO
    return std::string();
}

PlayerApple *PlayerApple::create(int id, std::string name, int score) {
    PlayerApple *ret = new(std::nothrow) PlayerApple();
    if (ret && ret->init(id, name, score)) {
        ret->retain();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool PlayerApple::init(int id, std::string name, int score) {
    if (!Player::init(id, name)) {
        return false;
    }

    _nameView = cocos2d::Label::createWithTTF(_name.c_str(), Variables::FONT_NAME, 32.f,
                                              cocos2d::Size(_visibleSize.width / 2 - 70.f, 0.f));

    _nameView->setAnchorPoint(Vec2::ZERO);

    _nameView->setPosition(0, _visibleSize.height - 40.f);

    _nameView->setColor(cocos2d::Color3B::BLACK);

    _scoreView = Node::create();
    _scoreView->setScale(0.5f);
    _scoreView->setPosition(Vec2(_scoreView->getBoundingBox().size.width / 2,
                                 _visibleSize.height - _scoreView->getBoundingBox().size.height / 2 - 95.f));

    auto coin = Sprite::createWithSpriteFrameName(Variables::COIN);

    coin->setPosition(coin->getBoundingBox().size.width / 2, coin->getBoundingBox().size.height / 2);

    _scoreView->addChild(coin, 2, "coin");

    auto coinsCount = cocos2d::Label::createWithTTF(StringUtils::toString(score), Variables::FONT_NAME,
                                                    Variables::H_FONT_SIZE);
    coinsCount->setColor(Color3B::BLACK);

    coinsCount->setPosition(coin->getBoundingBox().size.width + 10.f + coinsCount->getBoundingBox().size.width / 2,
                            coin->getBoundingBox().size.height / 2);

    _scoreView->addChild(coinsCount, 2, "count");

    this->addChild(_nameView, 1, "name");
    this->addChild(_scoreView, 1, "coins_view");

    return true;
}

void PlayerApple::addScore(int newCount) {
    Node *coin = _scoreView->getChildByName("count");
    Label *count = (Label *) _scoreView->getChildByName("count");
    count->setString(StringUtils::toString(newCount));
    count->setPosition(coin->getBoundingBox().size.width + 10.f,
                       count->getPosition().y);
}

void PlayerApple::_updateView() {

}

std::string PlayerApple::getPlayerView() {
    return std::string();
}

int PlayerApple::getHp() {
    return 100;
}

void PlayerApple::setHAlignment(cocos2d::TextHAlignment alignment) {

}

PlayerOnlineApple *PlayerOnlineApple::create(int id, std::string name, int score) {
    PlayerOnlineApple *ret = new(std::nothrow) PlayerOnlineApple();
    if (ret && ret->init(id, name, score)) {
        ret->retain();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool PlayerOnlineApple::init(int id, std::string name, int score) {
    if (!PlayerApple::init(id, name, score)) {
        return false;
    }
    _scoreView->removeFromParent();
    _scoreView = Node::create();
    _scoreView->setScale(0.5f);
    _scoreView->setPosition(Vec2(_scoreView->getBoundingBox().size.width / 2,
                                 _visibleSize.height - _scoreView->getBoundingBox().size.height / 2 - 95.f));

    auto coin = Sprite::createWithSpriteFrameName(Variables::COIN);

    coin->setPosition(coin->getBoundingBox().size.width / 2, coin->getBoundingBox().size.height / 2);

    _scoreView->addChild(coin, 2, "coin");

    auto coinsCount = cocos2d::Label::createWithTTF(StringUtils::toString(score), Variables::FONT_NAME,
                                                    Variables::H_FONT_SIZE);
    coinsCount->setColor(Color3B::BLACK);

    coinsCount->setPosition(coin->getBoundingBox().size.width + 10.f + coinsCount->getBoundingBox().size.width / 2,
                            coin->getBoundingBox().size.height / 2);

    _scoreView->addChild(coinsCount, 2, "count");

    this->addChild(_scoreView, 1, "coins_view");

    return true;
}

void PlayerOnlineApple::_updateView() {

}

std::string PlayerOnlineApple::getPlayerView() {
    return PlayerApple::getPlayerView();
}

Bot *Bot::create(std::string name, int hp) {
    auto id = BattleScene::getInstance()->getStickmanCount();
    Bot *ret = new(std::nothrow) Bot();
    if (ret && ret->init(id, name, hp)) {
        ret->retain();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool Bot::init(int id, std::string name, int hp) {
    if (!Player::init(id, name)) {
        return false;
    }

    _hp = hp;

    return true;
}

int Bot::getHp() {
    return _hp;
}

void Bot::setHp(int diff) {
    _hp -= diff;
    if (_hp > 0) {
        _hpView->setHp(_hp);
    }
}

void Bot::_updateView() {

}

void Bot::setHAlignment(cocos2d::TextHAlignment alignment) {

}

std::string Bot::getPlayerView() {
    return std::string();
}
