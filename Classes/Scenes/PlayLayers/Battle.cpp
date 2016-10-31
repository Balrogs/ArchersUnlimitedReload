#include <GameEngine/Objects/Stickman.h>
#include <GameEngine/Global/WeaponSelector.h>
#include <GameEngine/Global/Producer.h>
#include "Battle.h"
#include "AppleBattle.h"
#include "DuelScene.h"

USING_NS_CC;

Scene *BattleScene::createScene(int type) {
    auto scene = Scene::createWithPhysics();
    BattleScene *layer;
    switch (type) {
        case 0:
            layer = BattleScene::create();
            break;
        case 1:
            layer = AppleBattle::create();
            break;
        case 2:
            layer = DuelScene::create();
            break;
        default:
            layer = BattleScene::create();
    }

    UI *hud = UI::create();

    scene->addChild(layer, 3);
    scene->addChild(hud, 2);

    layer->ui = hud;

    layer->initWorld();

    return scene;
}

const float  BattleScene::MAX_ARROW_POWER = 50.f;
const float  BattleScene::MIN_ARROW_POWER = 5.f;

const float  BattleScene::MAX_ARROW_ANGLE = 1.5f;
const float  BattleScene::MIN_ARROW_ANGLE = -1.5f;

const float  BattleScene::GROUND = 100.f;
const float  BattleScene::G = -0.6f;
BattleScene *BattleScene::instance = nullptr;

bool BattleScene::init() {
    if (!LayerColor::init()) {
        return false;
    }
    BattleScene::instance = this;

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    GLOBAL_SCALE = 1.f;

    _player = nullptr;
    _bullet_pull = Node::create();
    _isPaused = false;
    this->addChild(_bullet_pull);
    const auto dragonBonesData = factory.loadDragonBonesData("Animation/5oct/ArcUnlim_2/ArcUnlim_2.json");
    factory.loadTextureAtlasData("Animation/5oct/ArcUnlim_2/texture.json");

    if (dragonBonesData) {
        cocos2d::Director::getInstance()->getScheduler()->schedule(
                schedule_selector(BattleScene::_enterFrameHandler),
                this, 0.f, false
        );

        const auto keyboardListener = cocos2d::EventListenerKeyboard::create();
        keyboardListener->onKeyPressed = CC_CALLBACK_2(BattleScene::_keyBoardPressedHandler, this);
        keyboardListener->onKeyReleased = CC_CALLBACK_2(BattleScene::_keyBoardReleasedHandler, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);


        const auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = CC_CALLBACK_2(BattleScene::_touchHandlerBegin, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(BattleScene::_touchHandlerMove, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(BattleScene::_touchHandlerEnd, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

        auto contactListener = EventListenerPhysicsContact::create();
        contactListener->onContactBegin = CC_CALLBACK_1(BattleScene::onContactBegin, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    } else {
        assert(false);
    }

    return true;
}


void BattleScene::removeBrain(Brain *brain) {
    if (!_brains.empty()) {
        brain->removeFromParent();
        _brains.erase(std::find(_brains.begin(), _brains.end(), brain));
    }
}

void BattleScene::addBrain(Brain *brain) {
    this->addChild(brain);
    _brains.push_back(brain);
}

void BattleScene::addTarget(DragonObject *target) {
    _targets.push_back(target);
}

void BattleScene::removeTarget(DragonObject *target) {
    if (!_targets.empty())
        _targets.erase(std::find(_targets.begin(), _targets.end(), target));
}


void BattleScene::_enterFrameHandler(float passedTime) {
    if (isGameOver()) {
        _onPopScene();
    }
    dragonBones::WorldClock::clock.advanceTime(passedTime);
}

void BattleScene::_onPopScene() {
    Director::getInstance()->popScene();
}

bool BattleScene::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    _player->startAim();
    return true;
}

bool BattleScene::_touchHandlerMove(const cocos2d::Touch *touch, cocos2d::Event *event) {

    const auto start = touch->getStartLocation();
    const auto curr = touch->getLocation();
    float x = start.x - curr.x;
    float y = start.y - curr.y;
    auto angle = std::atan2(y, x);
    auto power = std::sqrt(x * x + y * y);
    power = power / 10;
    power = (power > MAX_ARROW_POWER) ? MAX_ARROW_POWER : power;
    power = (power < MIN_ARROW_POWER) ? MIN_ARROW_POWER : power;
    angle = (angle > MAX_ARROW_ANGLE) ? MAX_ARROW_ANGLE : angle;
    angle = (angle < MIN_ARROW_ANGLE) ? MIN_ARROW_ANGLE : angle;
    _player->setAim(angle, power);
    return true;
}


bool BattleScene::_touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) {

    const auto start = touch->getStartLocation();
    const auto curr = touch->getLocation();
    float x = start.x - curr.x;
    float y = start.y - curr.y;
    auto power = std::sqrt(x * x + y * y);
    power = power / 10;
    power = (power > MAX_ARROW_POWER) ? MAX_ARROW_POWER : power;
    power = (power < MIN_ARROW_POWER) ? MIN_ARROW_POWER : power;
    auto angle = std::atan2(y, x);
    angle = (angle > MAX_ARROW_ANGLE) ? MAX_ARROW_ANGLE : angle;
    angle = (angle < MIN_ARROW_ANGLE) ? MIN_ARROW_ANGLE : angle;
    _player->attack(angle, power, x, y);
    return true;
}

void BattleScene::_keyBoardPressedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
}

void BattleScene::_keyBoardReleasedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_BREAK:
        case EventKeyboard::KeyCode::KEY_ESCAPE:
        case EventKeyboard::KeyCode::KEY_BACKSPACE:
            _onPopScene();
            break;
    }
}

bool BattleScene::onContactBegin(cocos2d::PhysicsContact &contact) {
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();

    if ((1 == a->getTag() && 2 == b->getTag()) ||
        (2 == a->getTag() && 1 == b->getTag())) {

        if (Arrow *v = dynamic_cast<Arrow *>(a->getNode())) {
            v->processContact(b->getNode());
        } else if (Arrow *va = dynamic_cast<Arrow *>(b->getNode())) {
            va->processContact(a->getNode());
        }
    }
    return true;
}

void BattleScene::initWorld() {

    Node *ground = Node::create();
    ground->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(0.f, BattleScene::GROUND + origin.y),
                                                          Vec2(visibleSize.width, BattleScene::GROUND + origin.y)));
    ground->getPhysicsBody()->setContactTestBitmask(true);
    this->addChild(ground);


    _player = new Hero(50.f + origin.x, BattleScene::GROUND + origin.y + 200.f, new Player(1, "hero"));

    ui->initBattle(visibleSize, _player);

    Producer *prod = new Producer("cre");
    prod->startLevel(1);
}

float BattleScene::getGlobalScale() {
    return this->GLOBAL_SCALE;
}

void BattleScene::_changeGlobalScale(float scale) {
    GLOBAL_SCALE = scale;
    _player->getDisplay()->setScale(_player->getNormalScale() * scale);
    for (auto target : _targets) {
        target->getDisplay()->setScale(target->getNormalScale() * scale);
    }
}

void BattleScene::moveScene(float x) {
    auto current_pos = this->getPosition();
    this->setPosition(Vec2(current_pos.x - x, current_pos.y));
}


void BattleScene::_pause() {
    if (_isPaused) {
        _unPause();
        return;
    }
    _isPaused = true;
    for (auto bullet: _bullet_pull->getChildren()) {
        bullet->pause();
    }
    this->pause();
}

void BattleScene::_unPause() {
    _isPaused = false;
    for (auto bullet: _bullet_pull->getChildren()) {
        bullet->resume();
    }
    this->resume();
}


cocos2d::Node *BattleScene::getBulletPull() {
    return this->_bullet_pull;
}

bool BattleScene::isIsPaused() const {
    return _isPaused;
}

bool BattleScene::isGameOver() {
    return false;
}

cocos2d::Vec2 BattleScene::getPlayerPos() {
    return Vec2(_player->getPosition().x, _player->getGlobalHeight("Head"));
}
