#include <Scenes/PlayLayers/DuelScene.h>
#include <Scenes/PlayLayers/DuelScene2P.h>
#include "Hero.h"
#include "Scenes/PlayLayers/Battle.h"
#include "GameEngine/Global/Variables.h"

Hero::Hero(float x_pos, float y_pos) : Hero(x_pos, y_pos, new Player(100, "HERO")) {
}

Hero::Hero(float x_pos, float y_pos, Player *player) : Body(x_pos, y_pos, 0.3f, 1),
                                                       _isAttacking(false),
                                                       _weaponIndex(0),
                                                       _weaponName(""),
                                                       _aimPowerState(nullptr),
                                                       _shoulders(nullptr) {
    _move_speed = 20.f;

    _aim = new Aim();
    _prevAim = nullptr;

    this->addChild(_aim);

    _player = player;

    WEAPON_LIST.push_back("Arrow");
    WEAPON_LIST.push_back("Arrow");
    WEAPON_LIST.push_back("Arrow");
    WEAPON_LIST.push_back("Arrow");
    WEAPON_LIST.push_back("Arrow");
    WEAPON_LIST.push_back("Arrow");
    WEAPON_LIST.push_back("BombArrow");
    WEAPON_LIST.push_back("Arrow");

    _weaponName = WEAPON_LIST[_weaponIndex];

    _armature = BattleScene::instance->factory.buildArmature("Stickman");
    _armatureDisplay = (dragonBones::CCArmatureDisplay *) _armature->getDisplay();

    _armature->getAnimation().fadeIn(Variables::STICKMAN_SETUP_ANIMATION);

    for (auto child : _armatureDisplay->getChildren()) {
        auto bone = _armature->getBoneByDisplay(child);
        if (bone == nullptr) {
            continue;
        }
        if (bone->name == "Head") {
            child->setPhysicsBody(cocos2d::PhysicsBody::createCircle(child->getBoundingBox().size.height / 2,
                                                                     cocos2d::PHYSICSBODY_MATERIAL_DEFAULT));
        } else {
            child->setPhysicsBody(cocos2d::PhysicsBody::createBox(child->getBoundingBox().size,
                                                                  cocos2d::PHYSICSBODY_MATERIAL_DEFAULT));
        }
        auto physicsBody = child->getPhysicsBody();
        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(0x01);
        physicsBody->setCollisionBitmask(0x02);
        physicsBody->setTag(2);
        physicsBody->setName(bone->name);
        physicsBody->setContactTestBitmask(true);
    }

    _armature->getAnimation().fadeIn(Variables::STICKMAN_IDLE_ANIMATION);


    _shoulders = _armature->getSlot("Shoulders")->getChildArmature();
    _shouldersDisplay = (dragonBones::CCArmatureDisplay *) _shoulders->getDisplay();


    const auto firePointBone = _armature->getBone("shoulders");
    cocos2d::Vec2 globalPoint;
    globalPoint.set(firePointBone->global.x, -firePointBone->global.y);
    _shouldersDisplay->setPosition(globalPoint);
    _shouldersDisplay->setAnchorPoint(globalPoint);

    _armature->removeBone(_armature->getBone("shoulders"));

    _aimPowerState = _shoulders->getAnimation().fadeIn(
            "setup", 0.f, 1,
            0, "aim", dragonBones::AnimationFadeOutMode::SameGroup
    );
    for (auto child : _shouldersDisplay->getChildren()) {
        auto bone = _shoulders->getBoneByDisplay(child);
        if (bone == nullptr) {
            continue;
        }
        auto physicsBody = cocos2d::PhysicsBody::createBox(child->getBoundingBox().size,
                                                           cocos2d::PHYSICSBODY_MATERIAL_DEFAULT);

        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(0x01);
        physicsBody->setCollisionBitmask(0x02);
        physicsBody->setTag(2);
        physicsBody->setName(bone->name);
        physicsBody->setContactTestBitmask(true);

        child->setPhysicsBody(physicsBody);
    }

    _aimPowerState = _shoulders->getAnimation().fadeIn(
            Variables::AIM_IDLE_ANIMATION, 0.f, 1,
            0, "aim", dragonBones::AnimationFadeOutMode::SameGroup
    );

    _bowArmature = _shoulders->getSlot("Bow")->getChildArmature();
    _bowArmatureDisplay = (dragonBones::CCArmatureDisplay *) _bowArmature->getDisplay();
    _arrowArmature = _shoulders->getSlot("Arrow")->getChildArmature();
    _arrowDisplay = (dragonBones::CCArmatureDisplay *) _arrowArmature->getDisplay();
    _arrowDisplay->setVisible(false);

    _string = cocos2d::Node::create();

    _updateString();

    _bowArmatureDisplay->addChild(_string);

    dragonBones::WorldClock::clock.add(_armature);
    this->addChild(_armatureDisplay);
    this->setPosition(_x_pos, _y_pos);
    this->setScale(BattleScene::instance->getGlobalScale());
    BattleScene::instance->addChild(this);
    BattleScene::instance->addStickman();

}

Hero::~Hero() {
}

void Hero::update() {
    _updatePosition();
}

void Hero::attack() {
    CCLOG("Attacking: angle %f power %f",_aim->get_aimRadian(), _aim->get_aimPower());
    attack(_aim->get_aimRadian(), _aim->get_aimPower());
}

void Hero::aim() {
    _aim->set_aiming(true);
    _aimPowerState = _shoulders->getAnimation().fadeIn(
            Variables::AIM_ANIMATION, 0.f, 1,
            0, "aim", dragonBones::AnimationFadeOutMode::SameGroup
    );
    _arrowDisplay->setVisible(true);
    auto hero_pos = BattleScene::instance->getPlayerPos();

    const auto firePointBone = _shoulders->getSlot("Bow");
    auto globalPoint = cocos2d::Vec2(this->getPosition().x, Variables::translatePoint(
            cocos2d::Vec3(firePointBone->global.x, -firePointBone->global.y, 0.f),
            _shouldersDisplay).y);

    float x = globalPoint.x - hero_pos.x;

    int period = (int) round(x / cocos2d::RandomHelper::random_real(16.f, 17.5f));

    //auto power = cocos2d::RandomHelper::random_real(BattleScene::MIN_ARROW_POWER + 20.f ,BattleScene::MAX_ARROW_POWER);

    auto power = BattleScene::MAX_ARROW_POWER;

    auto angle = -x / (power * period);
    CCLOG("AIMING: period %d x %f", period, x);
    angle = (float) acos(angle);
    angle = 360 * dragonBones::ANGLE_TO_RADIAN - angle;
    _aim->set_aimRadian(angle);
    _aim->set_aimPower(power);
    _updateAim();
}

void Hero::attack(float radian, float power) {
    if (_isAttacking) {
        return;
    }
    _isAttacking = true;
    radian = - radian;
    const auto firePointBone = _shoulders->getSlot("Bow");
    auto globalPoint = Variables::translatePoint(cocos2d::Vec3(firePointBone->global.x, -firePointBone->global.y, 0.f),
                                                 _shouldersDisplay);
    globalPoint.x = getPosition().x;
    int id = _player->getId();
    switch (_weaponIndex) {
        case 0:
            _fire(new Arrow(WEAPON_LIST[_weaponIndex], radian, power, globalPoint, id));
            break;
        case 1:
            _fire(new Arrow(WEAPON_LIST[_weaponIndex], radian + 3.f * dragonBones::ANGLE_TO_RADIAN, power, globalPoint,
                            id));
            _fire(new Arrow(WEAPON_LIST[_weaponIndex], radian - 3.f * dragonBones::ANGLE_TO_RADIAN, power, globalPoint,
                            id));
            break;

        case 2:
            _fire(new Arrow(WEAPON_LIST[_weaponIndex], radian + 6.f * dragonBones::ANGLE_TO_RADIAN, power, globalPoint,
                            id));
            _fire(new Arrow(WEAPON_LIST[_weaponIndex], radian, power, globalPoint, id));
            _fire(new Arrow(WEAPON_LIST[_weaponIndex], radian - 6.f * dragonBones::ANGLE_TO_RADIAN, power, globalPoint,
                            id));
            break;
        case 3:
            _fire(new PowerArrow(WEAPON_LIST[_weaponIndex], radian, power, globalPoint, id));
            break;
        case 4:
            _fire(new FrozenArrow(WEAPON_LIST[_weaponIndex], radian, power, globalPoint, id));
            break;
        case 5:
            _fire(new FireArrow(WEAPON_LIST[_weaponIndex], radian, power, globalPoint, id));
            break;
        case 6:
            _fire(new BombArrow(WEAPON_LIST[_weaponIndex], radian, power, globalPoint, id));
            break;
        case 7:
            _fire(new MineArrow(WEAPON_LIST[_weaponIndex], radian, power, globalPoint, id));
            break;
        case 8:
            _fire(new DuelArrow(WEAPON_LIST[_weaponIndex], radian, power, globalPoint, id));
            break;
        default:
            break;
    }
    _aimPowerState = _shoulders->getAnimation().fadeIn(
            Variables::SHOT_ANIMATION, 0.f, 1,
            0, "aim", dragonBones::AnimationFadeOutMode::SameGroup
    );
    _isAttacking = false;
}

void Hero::switchWeapon(int dest) {

    _weaponIndex += dest;

    if (_weaponIndex >= WEAPON_LIST.size()) {
        _weaponIndex = 0;
    } else if (_weaponIndex <= 0)
        _weaponIndex = (unsigned int) WEAPON_LIST.size() - 1;

    _shoulders->getSlot("Arrow")->setChildArmature(
            BattleScene::instance->factory.buildArmature(WEAPON_LIST[_weaponIndex]));
    _arrowArmature = _shoulders->getSlot("Arrow")->getChildArmature();
    _arrowDisplay = (dragonBones::CCArmatureDisplay *) _arrowArmature->getDisplay();
}


void Hero::_fire(Arrow *arrow) {
    UI::enableArrows(this, false);
    this->getPlayer()->addShotsCount();
    _saveAim();
    BattleScene::instance->getBulletPull()->addChild(arrow);
    _arrowDisplay->setVisible(false);
    _state = IDLE;
    _aimPowerState = _shoulders->getAnimation().fadeIn(
            Variables::AIM_IDLE_ANIMATION, 0.f, 1,
            0, "aim", dragonBones::AnimationFadeOutMode::All
    );
    _shouldersDisplay->setRotation(0.f);

    _updateString();
    setFaceDir();
}

void Hero::_updateAim() {
    if (!_aim->is_aiming()) {
        return;
    }
    CCLOG("AIM ANGLE : %f", _aim->get_aimRadian() * dragonBones::RADIAN_TO_ANGLE);
    const auto firePointBone = _shoulders->getSlot("Bow");
    auto globalPoint = Variables::translatePoint(cocos2d::Vec3(firePointBone->global.x, -firePointBone->global.y, 0.f),
                                                 _shouldersDisplay, this);
    _aim->set_aimPoint(globalPoint);

    _aimPowerState = _shoulders->getAnimation().gotoAndStopByTime(Variables::AIM_ANIMATION, _aim->get_aimPower() / 50);

    float angle = _aim->get_aimRadian() * dragonBones::RADIAN_TO_ANGLE;

    if(_faceDir < 0)
        angle = 180 - angle;

    _aim->set_aimRadian(angle * dragonBones::ANGLE_TO_RADIAN);
    _shouldersDisplay->setRotation(angle);

    _shoulders->invalidUpdate("", true);

    _updateString();
}

void Hero::setAim(float angle, float power) {
    _aim->set_aimRadian(-angle);
    _aim->set_aimPower(power);
    _updateAim();
}

void Hero::startAim() {
    _aim->set_aiming(true);
    _aimPowerState = _shoulders->getAnimation().fadeIn(
            Variables::AIM_ANIMATION, 0.f, 1,
            0, "aim", dragonBones::AnimationFadeOutMode::SameGroup
    );
    _arrowDisplay->setVisible(true);

    _updateAim();
}


void Hero::_updateAnimation() {
    if (_state == MOVING) {
        _armature->getAnimation().fadeIn(Variables::STICKMAN_WALK_ANIMATION);
    } else {
        _armature->getAnimation().fadeIn(Variables::STICKMAN_IDLE_ANIMATION);
    }
}

void Hero::_updateString() {

    _string->removeAllChildren();

    auto top = _bowArmature->getBone("top");
    auto bottom = _bowArmature->getBone("bottom");

    if (_aim->is_aiming()) {

        auto firePoint = _shoulders->getBone("arrow");

        auto globalPoint = Variables::translatePoint(cocos2d::Vec3(firePoint->global.x, firePoint->global.y, 0.f),
                                                     _shouldersDisplay, _bowArmatureDisplay);

        auto line1 = cocos2d::DrawNode::create();
        auto line2 = cocos2d::DrawNode::create();

        line1->drawLine(cocos2d::Vec2(top->global.x, -top->global.y),
                        cocos2d::Vec2(globalPoint.x, globalPoint.y),
                        cocos2d::Color4F::BLACK);

        line2->drawLine(cocos2d::Vec2(globalPoint.x, globalPoint.y),
                        cocos2d::Vec2(bottom->global.x, -bottom->global.y),
                        cocos2d::Color4F::BLACK);

        _string->addChild(line1, 1);
        _string->addChild(line2, 1);
    } else {
        auto line = cocos2d::DrawNode::create();
        line->drawLine(cocos2d::Vec2(top->global.x, -top->global.y), cocos2d::Vec2(bottom->global.x, -bottom->global.y),
                       cocos2d::Color4F::BLACK);
        _string->addChild(line, 1);
    }
}

Player *Hero::getPlayer() const {
    return _player;
}

std::vector<std::string> Hero::getWeaponList() {
    return WEAPON_LIST;
}

void Hero::_saveAim() {
    if (_prevAim != nullptr) {
        this->removeChild((cocos2d::Node *) _prevAim, true);
    }
    _prevAim = _aim;
    _prevAim->disable();
    _aim = new Aim();
    this->addChild(_aim);
}

void Hero::setFaceDir() {
    int facedir = 1;
    if(DuelScene2P* scene = dynamic_cast<DuelScene2P*>(BattleScene::instance)) {
        if(scene->getHeroPos(scene->getHero(_player->getId())).x < getPosition().x){
            facedir = -1;
        }
    }
    changeFacedir(facedir);
}

DuelHero::DuelHero(float x_pos, float y_pos, const char *name) : Hero(x_pos, y_pos, new Player(100, name)) {
    _weaponIndex = 8;
    WEAPON_LIST.push_back("Arrow");
}

void DuelHero::switchWeapon(int i) {
    Hero::switchWeapon(i);
}

void DuelHero::move(int dir) {
    setState(MOVING);
    auto movedir = dir * _faceDir;
    changeFacedir(movedir);
    cocos2d::Vec3 pos = cocos2d::Vec3(this->getPosition().x + movedir * 150.f, this->getPosition().y, 0.f);

    this->runAction(cocos2d::Sequence::create(
            cocos2d::CallFunc::create([&]() {
                if (_prevAim != nullptr)
                    _prevAim->setVisible(false);
            }),
            cocos2d::MoveTo::create(1.5f, pos),
            cocos2d::CallFunc::create([&]() {
                setState(IDLE);
                setFaceDir();
                if (_prevAim != nullptr)
                    _prevAim->setVisible(true);
            }),
            NULL)
    );
}


AppleHero::AppleHero(float x_pos, float y_pos, const char *name) : Hero(x_pos, y_pos, new Player(100, name)) {
    _weaponIndex = 0;
    WEAPON_LIST.push_back("Arrow");
}

void AppleHero::_saveAim() {
    _aim->set_aiming(false);
}

