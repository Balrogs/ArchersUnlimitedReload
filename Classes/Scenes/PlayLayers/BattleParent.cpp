#include "BattleParent.h"
#include <GameEngine/Global/Misc/PopUp.h>
#include <Localization/LocalizedStrings.h>
#include <Scenes/PlayLayers/Apple/AppleMultiplayer.h>
#include "Battle.h"
#include "Scenes/PlayLayers/Apple/AppleBattle.h"
#include "Scenes/PlayLayers/Duel/DuelScene.h"
#include "Scenes/PlayLayers/Duel/DuelScene2P.h"
#include "Scenes/PlayLayers/Duel/DuelSceneMultiplayer.h"
#include "GameOverScene.h"

USING_NS_CC;



const float  BattleParent::MAX_ARROW_POWER = 25.f;
const float  BattleParent::MIN_ARROW_POWER = 1.f;
float  BattleParent::GROUND = 0.f;
const float  BattleParent::G = -0.15f;

BattleParent* BattleParent::_instance = nullptr;

BattleParent *BattleParent::getInstance() {
    return _instance;
}

Scene *BattleParent::createScene(int type) {
    Statistics *stats = Statistics::create(type);
    return createScene(stats);
}

cocos2d::Scene *BattleParent::createScene(Statistics *stats) {
    auto scene = Scene::createWithPhysics();
    BattleParent *layer;
    switch (stats->getType()) {
        case 0:
            layer = BattleScene::create(stats);
            break;
        case 1:
            layer = AppleBattle::create(stats);
            break;
        case 2:
            layer = DuelScene::create(stats);
            break;
        case 3:
            layer = DuelScene2P::create(stats);
            break;
        case 4:
            layer = DuelSceneMultiplayer::create(stats);
            break;
        case 5:
            layer = AppleMultiplayer::create(stats);
            break;
        default:
            layer = BattleScene::create(stats);
    }

    UI *hud = UI::create();

    int id = RandomHelper::random_int(1, 3);
    BackgroundLayer *bg = BackgroundLayer::create(id);

    scene->addChild(bg, 2);
    scene->addChild(layer, 3);
    scene->addChild(hud, 4);

    layer->_ui = hud;
    layer->_bg = bg;

    layer->initWorld();

    return scene;
}


bool BattleParent::init(Statistics *stats) {
    if (!LayerColor::init()) {
        return false;
    }

    // global vars
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    _GLOBAL_SCALE = 1.f;
    GROUND = 100.f;

    //disable multitouch
    _touch = -1;

    //init battle info
    _stats = stats;

    //children initialization
    _player = nullptr;
    _bullet_pull = Node::create();
    _isPaused = false;
    this->addChild(_bullet_pull);

    const auto dragonBonesData = factory.loadDragonBonesData("ArcUnlimArmature.json");
    factory.loadTextureAtlasData("texture.json");


    if (dragonBonesData) {
        cocos2d::Director::getInstance()->getScheduler()->schedule(
                schedule_selector(BattleParent::_enterFrameHandler),
                this, 0.f, false
        );

        const auto keyboardListener = cocos2d::EventListenerKeyboard::create();
        keyboardListener->onKeyPressed = CC_CALLBACK_2(BattleParent::_keyBoardPressedHandler, this);
        keyboardListener->onKeyReleased = CC_CALLBACK_2(BattleParent::_keyBoardReleasedHandler, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);


        const auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = CC_CALLBACK_2(BattleParent::_touchHandlerBegin, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(BattleParent::_touchHandlerMove, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(BattleParent::_touchHandlerEnd, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

        auto contactListener = EventListenerPhysicsContact::create();
        contactListener->onContactBegin = CC_CALLBACK_1(BattleParent::onContactBegin, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);


    } else {
        assert(false);
    }

    return true;
}

void BattleParent::_enterFrameHandler(float passedTime) {
    if (isGameOver()) {
        _gameOver();
    }
    dragonBones::WorldClock::clock.advanceTime(passedTime);
}

void BattleParent::onPopScene() {
    this->_bg->removeSprites();
    Director::getInstance()->popScene();
}

bool BattleParent::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_touch < 0) {
        _touch = touch->getID();
    } else {
        return false;
    }

    if (_isPaused)
        return false;
    const auto start = touch->getStartLocation();
    if (this->_ui->checkTouch(start)) {
        return false;
    }
    _player->startAim();
    return true;
}

bool BattleParent::_touchHandlerMove(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_touch != touch->getID()) {
        return false;
    }

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


bool BattleParent::_touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) {

    if (_touch == touch->getID()) {
        _touch = -1;
    } else {
        return false;
    }

    const auto start = touch->getStartLocation();
    const auto curr = touch->getLocation();
    float x = start.x - curr.x;
    float y = start.y - curr.y;
    auto power = std::sqrt(x * x + y * y);
    power = power / 10;
    power = (power > MAX_ARROW_POWER) ? MAX_ARROW_POWER : power;
    power = (power < MIN_ARROW_POWER) ? MIN_ARROW_POWER : power;
    auto angle = std::atan2(y, x);
    _player->attack(-angle, power);
    return true;
}

void BattleParent::_keyBoardPressedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
}

void BattleParent::_keyBoardReleasedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
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

bool BattleParent::onContactBegin(cocos2d::PhysicsContact &contact) {
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

float BattleParent::getGlobalScale() {
    return this->_GLOBAL_SCALE;
}

void BattleParent::pauseBattle() {
    _isPaused = true;
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0);
    _pauseRecursive(this, _isPaused);
    this->resume();
}

void BattleParent::_pauseRecursive(Node *_node, bool _pause) {
    if (_pause)
        _node->pause();
    else
        _node->resume();

    auto &children = _node->getChildren();
    for (size_t i = 0; i < children.size(); i++) {
        _pauseRecursive(children.at(i), _pause);
    }
}

void BattleParent::unPause() {
    _isPaused = false;
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(1);
    _pauseRecursive(this, _isPaused);
}


cocos2d::Node *BattleParent::getBulletPull() {
    return this->_bullet_pull;
}

bool BattleParent::isPaused() const {
    return _isPaused;
}

bool BattleParent::isGameOver() {
    return false;
}

cocos2d::Vec2 BattleParent::getHeroPos(Hero *player) {
    return Vec2(player->getPosition().x, player->getGlobalHeight("Head"));
}

int BattleParent::getStickmanCount() {
    return _stickmanCount;
}

void BattleParent::addStickman() {
    _stickmanCount++;
}

Vec2 BattleParent::getPlayerPos() {
    return Vec2(_player->getPosition().x, _player->getGlobalHeight("Head"));
}

void BattleParent::showPopUp() {
    auto popUp = this->_ui->getChildByName("PopUp");
    if (popUp == nullptr) {
        pauseBattle();
        popUp = PausePopUp::create(LocalizedStrings::getInstance()->getString("PAUSE"));
        popUp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        this->_ui->addChild(popUp, 0, "PopUp");
    } else {
        unPause();
        popUp->removeFromParent();
    }
}

Hero *BattleParent::getPlayer() {
    return _player;
}

void BattleParent::_gameOver() {

    this->_bg->removeSprites();

    this->getEventDispatcher()->removeEventListenersForTarget(this);
    this->getScheduler()->unscheduleAllForTarget(this);
    this->getActionManager()->removeAllActionsFromTarget(this);

    this->getParent()->addChild(GameOverScene::create(_stats), 5);
}

BackgroundLayer *BattleParent::getBackground() {
    return _bg;
}

void BattleParent::_createEnvForStickman(Body *stickman, int type) {
    if (stickman == nullptr) {
        return;
    }

    switch (type) {
        case -1: {
            auto random = RandomHelper::random_int(0, 4);
            _createEnvForStickman(stickman, random);
            break;
        }
        case 0: {
            break;
        }
        case 1: {

            break;
        }
        case 2: {

            break;
        }
        case 3: {

            break;
        }
        case 4: {

            break;
        }
        default: {
            break;
        }
    }
}

void BattleParent::addCoins(int value) {
    int diff = _getGainedCoinsByActionType(value);
    _stats->increaseCoins(diff);
    if (PlayerApple *pl = dynamic_cast<PlayerApple *>(_player->getPlayer())) {
        pl->addScore(_stats->getCoinsGained());
    }
}