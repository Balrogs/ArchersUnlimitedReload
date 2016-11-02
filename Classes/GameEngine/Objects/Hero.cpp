#include "Hero.h"
#include "Scenes/PlayLayers/Battle.h"
#include "GameEngine/Global/Variables.h"

Hero::Hero(float x_pos, float y_pos, Player *player) : Body(x_pos, y_pos, 0.3f, 100),
                                                       _isAttacking(false),
                                                       _weaponIndex(0),
                                                       _weaponName(""),
                                                       _aimPowerState(nullptr),
                                                       _shoulders(nullptr) {

    _aim = new Aim();
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

    _armature->removeBone(_armature->getBone("shoulders"));

    _shoulders->getAnimation().fadeIn("setup");
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

    _shoulders->getAnimation().fadeIn("idle");


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

}

Hero::~Hero() {
}

void Hero::update() {
    _updatePosition();
}

void Hero::attack() {
    const auto firePointBone = _shoulders->getSlot("Bow");
    auto globalPoint = Variables::translatePoint(cocos2d::Vec3(firePointBone->global.x, -firePointBone->global.y, 0.f),
                                                 _shouldersDisplay);
    _fire(new DuelArrow("Arrow", _aim->get_aimRadian(), _aim->get_aimPower(),
                        cocos2d::Vec2(this->getPosition().x, globalPoint.y), this->getPlayer()->getId()));
    _shoulders->getAnimation().fadeIn(Variables::SHOT_ANIMATION, 0.f, 1);
}

void Hero::aim() {
    _aim->set_aiming(true);
    _aimPowerState = _shoulders->getAnimation().fadeIn(Variables::AIM_ANIMATION, 0.f, 1);
    _arrowDisplay->setVisible(true);
    auto hero_pos = BattleScene::instance->getPlayerPos();

    const auto firePointBone = _shoulders->getSlot("Bow");
    auto globalPoint = cocos2d::Vec2(this->getPosition().x, Variables::translatePoint(
            cocos2d::Vec3(firePointBone->global.x, -firePointBone->global.y, 0.f),
            _shouldersDisplay).y);

    float x = hero_pos.x - globalPoint.x;
    float y = hero_pos.y - globalPoint.y;
    auto power = std::sqrt(x * x + y * y);

    CCLOG("Pow %f x %f y %f", power, x, y);

    if (power / 10 > BattleScene::MAX_ARROW_POWER) {
        auto npower = BattleScene::MAX_ARROW_POWER * 10;
        auto del = npower / power;
        x = x * del;
        y = std::sqrt(npower * npower - x * x);
        power = npower / 10;

    } else if (power / 10 < BattleScene::MIN_ARROW_POWER) {
        power = BattleScene::MIN_ARROW_POWER;
    }

    auto angle = 360 - std::atan2(y, x) * dragonBones::RADIAN_TO_ANGLE;
    angle = angle * dragonBones::ANGLE_TO_RADIAN;
    CCLOG("Rad %f Pow %f x %f y %f", angle, power, x, y);
    _aim->set_aimRadian(angle);
    _aim->set_aimPower(power);
    _updateAim();
}

void Hero::attack(float angle, float power, float x, float y) {
    if (_isAttacking) {
        return;
    }
    _isAttacking = true;
    const auto firePointBone = _shoulders->getSlot("Bow");
    auto globalPoint = Variables::translatePoint(cocos2d::Vec3(firePointBone->global.x, -firePointBone->global.y, 0.f),
                                                 _shouldersDisplay);


    auto radian = -angle;
    int id = _player->getId();

    switch (_weaponIndex) {
        case 0:
            _fire(new Arrow(WEAPON_LIST[_weaponIndex], radian, power, globalPoint, id));
            break;
        case 1:
            _fire(new Arrow(WEAPON_LIST[_weaponIndex], radian + 3.f * dragonBones::ANGLE_TO_RADIAN, power, globalPoint, id));
            _fire(new Arrow(WEAPON_LIST[_weaponIndex], radian - 3.f * dragonBones::ANGLE_TO_RADIAN, power, globalPoint, id));
            break;

        case 2:
            _fire(new Arrow(WEAPON_LIST[_weaponIndex], radian + 6.f * dragonBones::ANGLE_TO_RADIAN, power, globalPoint, id));
            _fire(new Arrow(WEAPON_LIST[_weaponIndex], radian, power, globalPoint, id));
            _fire(new Arrow(WEAPON_LIST[_weaponIndex], radian - 6.f * dragonBones::ANGLE_TO_RADIAN, power, globalPoint, id));
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
    }
    _shoulders->getAnimation().fadeIn(Variables::SHOT_ANIMATION, 0.f, 1);
    _isAttacking = false;
}

void Hero::switchWeapon(int dest) {
    _weaponIndex+=dest;
    if (_weaponIndex >= WEAPON_LIST.size()) {
        _weaponIndex = 0;
    }
    _shoulders->getSlot("Arrow")->setChildArmature(
            BattleScene::instance->factory.buildArmature(WEAPON_LIST[_weaponIndex]));
    _arrowArmature = _shoulders->getSlot("Arrow")->getChildArmature();
    _arrowDisplay = (dragonBones::CCArmatureDisplay *) _arrowArmature->getDisplay();
}


void Hero::_fire(Arrow *arrow) {
    this->getPlayer()->addShotsCount();
    _aim->set_aiming(false);
    BattleScene::instance->getBulletPull()->addChild(arrow);
    _arrowDisplay->setVisible(false);
    _state = IDLE;
    _aimPowerState = _shoulders->getAnimation().fadeIn(Variables::AIM_IDLE_ANIMATION);
    _aimPowerState->weight = 0;
    _shouldersDisplay->setRotation(0.f);

    _updateString();
}

void Hero::_updateAim() {
    if (!_aim->is_aiming()) {
        return;
    }

    const auto firePointBone = _shoulders->getSlot("Bow");
    auto globalPoint = Variables::translatePoint(cocos2d::Vec3(firePointBone->global.x, -firePointBone->global.y, 0.f),
                                                 _shouldersDisplay);


    _aim->set_aimPoint(globalPoint);

    _faceDir = _aim->get_aimRadian() > 1.5f || _aim->get_aimRadian() < -1.5f ? -1 : 1;

    _shouldersDisplay->getAnimation().gotoAndStopByTime(Variables::AIM_ANIMATION, _aim->get_aimPower() / 50);

//    if (_faceDir > 0)
//        _shouldersDisplay->setRotation(_aim->get_aimRadian() * dragonBones::RADIAN_TO_ANGLE);
//    else
//        _shouldersDisplay->setRotation(180 - _aim->get_aimRadian() * dragonBones::RADIAN_TO_ANGLE);

    _updateString();
}

void Hero::setAim(float angle, float power) {
    _aim->set_aimRadian(-angle);
    _aim->set_aimPower(power);

    _updateAim();
}

void Hero::startAim() {
    _aim->set_aiming(true);
    _aimPowerState = _shoulders->getAnimation().fadeIn(Variables::AIM_ANIMATION, -1.f, 1);
    _arrowDisplay->setVisible(true);

    _updateAim();
}

void Hero::_updateAnimation() {
    if (_state == IDLE) {
        return;
    }
    if (_moveDir == 0) {
        _armature->getAnimation().fadeIn(Variables::STICKMAN_IDLE_ANIMATION);
    } else {
        _armature->getAnimation().fadeIn(Variables::STICKMAN_WALK_ANIMATION);
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

        _string->addChild(line1);
        _string->addChild(line2);
    } else {
        auto line = cocos2d::DrawNode::create();
        line->drawLine(cocos2d::Vec2(top->global.x, -top->global.y), cocos2d::Vec2(bottom->global.x, -bottom->global.y),
                       cocos2d::Color4F::BLACK);
        _string->addChild(line);
    }
}

Player *Hero::getPlayer() const {
    return _player;
}

unsigned Hero::weaponIndex() {
    return _weaponIndex;
}

std::vector<std::string> Hero::getWeaponList() {
    return WEAPON_LIST;
}

DuelHero::DuelHero(float x_pos, float y_pos) : DuelHero(x_pos, y_pos, new Player(1, "BOT")) {

}

DuelHero::DuelHero(float x_pos, float y_pos, Player *player) : Hero(x_pos, y_pos, player) {
    _weaponIndex = 8;
    WEAPON_LIST.push_back("Arrow");
}

void DuelHero::switchWeapon(int dest) {
    Hero::switchWeapon(dest);
}
