#include <GameEngine/Objects/Stickman.h>
#include <GameEngine/Global/WeaponSelector.h>
#include <GameEngine/Global/Producer.h>
#include <GameEngine/Global/Misc/PopUp.h>
#include <GameEngine/Global/Variables.h>
#include <Scenes/Layers/BackgroundLayer.h>
#include "Battle.h"
#include "AppleBattle.h"
#include "DuelScene.h"
#include "DuelScene2P.h"
#include "DuelSceneMultiplayer.h"

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
        case 3:
            layer = DuelScene2P::create();
            break;
        case 4:
            layer = DuelSceneMultiplayer::create();
            break;
        default:
            layer = BattleScene::create();
    }

    UI *hud = UI::create();

    int id = 3;
    //int id = RandomHelper::random_int(1, 3);
    BackgroundLayer *bg = BackgroundLayer::create(id);

    scene->addChild(bg, 2);
    scene->addChild(layer, 3);
    scene->addChild(hud, 4);

    layer->_ui = hud;
    layer->_bg = bg;

    layer->initWorld();

    return scene;
}

const float  BattleScene::MAX_ARROW_POWER = 40.f;
const float  BattleScene::MIN_ARROW_POWER = 5.f;

const float  BattleScene::G = -0.5f;
BattleScene *BattleScene::instance = nullptr;

bool BattleScene::init() {
    if (!LayerColor::init()) {
        return false;
    }
    BattleScene::instance = this;
    GROUND = Director::getInstance()->getVisibleSize().height / 6;
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    _GLOBAL_SCALE = 1.f;

    _player = nullptr;
    _bullet_pull = Node::create();
    _isPaused = false;
    this->addChild(_bullet_pull);


    const auto dragonBonesData = factory.loadDragonBonesData("ArcUnlim_2.json");
    factory.loadTextureAtlasData("texture.json");


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

void BattleScene::addTarget(cocos2d::Node *target) {
    _targets.push_back(target);
}

void BattleScene::removeTarget(cocos2d::Node *target) {
    if (!_targets.empty())
        _targets.erase(std::find(_targets.begin(), _targets.end(), target));
}


void BattleScene::_enterFrameHandler(float passedTime) {
    if (isGameOver()) {
        onPopScene();
    }
    dragonBones::WorldClock::clock.advanceTime(passedTime);
}

void BattleScene::onPopScene() {
    this->_bg->removeSprites();
    Director::getInstance()->popScene();
}

bool BattleScene::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_isPaused)
        return false;
    const auto start = touch->getStartLocation();
    if (this->_ui->checkTouch(start)) {
        return false;
    }
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
    _player->attack(angle, power);
    return true;
}

void BattleScene::_keyBoardPressedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
}

void BattleScene::_keyBoardReleasedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_BREAK:
        case EventKeyboard::KeyCode::KEY_ESCAPE:
        case EventKeyboard::KeyCode::KEY_BACKSPACE: {
            showPopUp();
        }
            break;
        default:
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

    _player = new Hero(50.f + origin.x, BattleScene::GROUND + origin.y + 200.f);

    _ui->initBattle(visibleSize, _player);

    Producer *prod = new Producer("cre");

    prod->startLevel(1);
}

float BattleScene::getGlobalScale() {
    return this->_GLOBAL_SCALE;
}

void BattleScene::moveScene(float x) {
    auto current_pos = this->getPosition();
    this->setPosition(Vec2(current_pos.x - x, current_pos.y));
    this->_bg->move(x);
}

void BattleScene::pauseBattle() {
    _isPaused = true;
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0);
    _pauseRecursive(this, _isPaused);
    this->resume();
}

void BattleScene::_pauseRecursive(Node *_node, bool _pause) {
    if (_pause)
        _node->pause();
    else
        _node->resume();

    auto &children = _node->getChildren();
    for (size_t i = 0; i < children.size(); i++) {
        _pauseRecursive(children.at(i), _pause);
    }
}

void BattleScene::unPause() {
    _isPaused = false;
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(1);
    _pauseRecursive(this, _isPaused);
}


cocos2d::Node *BattleScene::getBulletPull() {
    return this->_bullet_pull;
}

bool BattleScene::isPaused() const {
    return _isPaused;
}

bool BattleScene::isGameOver() {
    return false;
}

cocos2d::Vec2 BattleScene::getHeroPos(Hero *player) {
    return Vec2(player->getPosition().x, player->getGlobalHeight("Head"));
}

int BattleScene::getStickmanCount() {
    return _stickmanCount;
}

void BattleScene::addStickman() {
    _stickmanCount++;
}

Vec2 BattleScene::getPlayerPos() {
    return Vec2(_player->getPosition().x, _player->getGlobalHeight("Head"));
}

void BattleScene::showPopUp() {
    auto popUp = this->_ui->getChildByName("PopUp");
    if (popUp == nullptr) {
        pauseBattle();
        popUp = PausePopUp::create("PAUSE");
        popUp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        this->_ui->addChild(popUp, 0, "PopUp");
    } else {
        unPause();
        popUp->removeFromParent();
    }
}

Hero *BattleScene::getPlayer() {
    return _player;
}

