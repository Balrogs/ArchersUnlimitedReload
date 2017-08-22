#include <Scenes/PlayLayers/Battle.h>
#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/JSONParser.h>
#include <iostream>
#include <iomanip>

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
    _font_2 =  Variables::FONT_SIZE();

    _counter = cocos2d::Label::createWithTTF(StringUtils::format("%d%%", _hp), Variables::FONT_NAME, _font_2);
    _counter->setColor(Color3B::BLACK);
    _counter->setPosition(Vec2(_size.width - 1.2f * _font_2, size.height / 2));

    this->addChild(_counter, 2);

    setHp(_hp);

    return true;
}

void HPBar::setHp(int hp) {
    _hp = hp;

    _counter->setString(StringUtils::format("%d%%", hp));

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
        if(percents < 0.8f) _counter->runAction(MoveTo::create(0.2f, Vec2(_size.width - new_Size - _font_2, _counter->getPositionY())));
    } else {
        _bar->runAction(ScaleTo::create(0.2f, x, y));
        if(percents < 0.8f) _counter->runAction(MoveTo::create(0.2f, Vec2(new_Size + _font_2, _counter->getPositionY())));
    }
}

void HPBar::setAlignment(bool isLeft) {
    _alignment = isLeft;
    if(!isLeft)
        _counter->setPositionX(1.2f * _font_2);
    setHp(_hp);
}

void HPBar::changeState(HPState state) {
    if (_state == state) {
        return;
    }

    if (_bar != nullptr){
        _bar->removeFromParent();
    }
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

    this->addChild(_bar, 1, "bar");
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

void PlayerDuel::markTurn(bool p) {
    if(p){
        _nameView->runAction(RepeatForever::create(
                Sequence::createWithTwoActions(
                        DelayTime::create(1.f),
                        Sequence::createWithTwoActions(
                                CallFunc::create(
                                        [&]() {
                                            _nameView->setColor(Color3B::BLUE);
                                        }
                                ),
                                Sequence::createWithTwoActions(
                                        DelayTime::create(1.f),
                                        CallFunc::create(
                                                [&]() {
                                                    _nameView->setColor(Color3B::WHITE);
                                                }
                                        )
                                )))
                             )
        );
    } else {
        _nameView->stopAllActions();
        _nameView->setColor(Color3B::BLACK);
    }

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

    _nameView = cocos2d::Label::createWithTTF(_name.c_str(), Variables::FONT_NAME, Variables::FONT_SIZE(),
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

    auto coinsCount = cocos2d::Label::createWithTTF("",
                                                    Variables::FONT_NAME,
                                                    Variables::H_FONT_SIZE(),
                                                    Size(Variables::H_FONT_SIZE() * 6, Variables::H_FONT_SIZE()),
                                                    TextHAlignment::RIGHT);

    coinsCount->setColor(Color3B::BLACK);

    coinsCount->setPosition(coin->getBoundingBox().size.width + 10.f,
                            coin->getBoundingBox().size.height / 2);

    _scoreView->addChild(coinsCount, 2, "count");

    addScore(score);

    this->addChild(_nameView, 1, "name");
    this->addChild(_scoreView, 1, "coins_view");

    setHAlignment(TextHAlignment::LEFT);

    return true;
}

void PlayerApple::addScore(int newCount) {
    Label *count = (Label *) _scoreView->getChildByName("count");
    string str = StringUtils::format("%d", newCount);
    std::ostringstream ss;
    ss << str << std::setw(6 - str.length()) << std::setfill(' ') << "";
    std::string s4(ss.str());
    count->setString(s4);
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

    _nameView->setHorizontalAlignment(alignment);

    if(auto count = dynamic_cast<Label *>(_scoreView->getChildByName("count"))){
        if(alignment == TextHAlignment::LEFT){
        count->setHorizontalAlignment(TextHAlignment::RIGHT);
        } else {
            count->setHorizontalAlignment(TextHAlignment::LEFT);
        }
    }
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
    _scoreView->setPosition(_nameView->getPosition());

    auto coinsCount = cocos2d::Label::createWithTTF(StringUtils::toString(score), Variables::FONT_NAME,
                                                    1.5f * Variables::H_FONT_SIZE(),
                                                    cocos2d::Size(_visibleSize.width / 2 - 70.f, 0.f));
    coinsCount->setColor(Color3B::BLACK);
    coinsCount->setPosition(Vec2(
            coinsCount->getContentSize().width / 2,
            0
    ));

    _scoreView->addChild(coinsCount, 2, "count");

    this->addChild(_scoreView, 1, "coins_view");

    setHAlignment(TextHAlignment::LEFT);

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


PlayerView *PlayerView::defaultView() {
    return new PlayerView(0, 0, 0);
}

PlayerView *PlayerView::randomBotView() {
    int hat = RandomHelper::random_int(0, (int) JSONParser::parseAsset("hats").size());
    int bow = RandomHelper::random_int(0, (int) JSONParser::parseAsset("bows").size());
    int arrow = RandomHelper::random_int(0, (int) JSONParser::parseAsset("arrows").size());
    return new PlayerView(hat, bow, arrow);
}


PlayerView *PlayerView::readPlayerView() {
    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    int hat = def->getIntegerForKey("hat", 3);
    int bow = def->getIntegerForKey("bow", 2);
    int arrow = def->getIntegerForKey("arrow", 3);

    return new PlayerView(hat, bow, arrow);
}

PlayerView::PlayerView(int hat, int bow, int arrow) {
    _hatId = hat;
    _bowId = bow;
    _arrowId = arrow;
}

AssetInfo* PlayerView::getHat() {
    return JSONParser::parseAsset("hats", _hatId);
}

AssetInfo* PlayerView::getBow() {
    return JSONParser::parseAsset("bows", _bowId);
}

AssetInfo* PlayerView::getArrow() {
    return JSONParser::parseAsset("arrows", _arrowId);
}

std::string PlayerView::toString() {
    return StringUtils::format("{\"hat\":%d,\"bow\":%d,\"arrow\":%d}", _hatId, _arrowId, _bowId);
}

void PlayerView::setHat(int id) {
    _hatId = id;
    cocos2d::UserDefault::getInstance()->setIntegerForKey("hat", _hatId);
}

void PlayerView::setBow(int id) {
    _bowId = id;
    cocos2d::UserDefault::getInstance()->setIntegerForKey("bow", _bowId);
}

void PlayerView::setArrow(int id) {
    _arrowId = id;
    cocos2d::UserDefault::getInstance()->setIntegerForKey("arrow", _arrowId);
}
