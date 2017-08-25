#include <GameEngine/Objects/Aim/Aim.h>
#include <GameEngine/Global/Variables.h>
#include <Scenes/PlayLayers/BattleParent.h>
#include <Scenes/PlayLayers/Duel/DuelScene2P.h>
#include "Hero.h"
#include "Stickman.h"


Hero::Hero(float x_pos, float y_pos) : Body(x_pos, y_pos, 0.3f, 1, PlayerView::readPlayerView()),
                                                       _isAttacking(false),
                                                       _weaponIndex(0),
                                                       _weaponName(""),
                                                       _aimPowerState(nullptr),
                                                       _shoulders(nullptr) {
    _move_speed = 20.f;

    _aim = new Aim();
    _prevAim = nullptr;

    this->addChild(_aim);

    WEAPON_LIST.push_back("Arrow");
    WEAPON_LIST.push_back("Arrow");
    WEAPON_LIST.push_back("Arrow");
    WEAPON_LIST.push_back("Arrow");
    WEAPON_LIST.push_back("Arrow");
    WEAPON_LIST.push_back("Arrow");
    WEAPON_LIST.push_back("BombArrow");
    WEAPON_LIST.push_back("Arrow");
    WEAPON_LIST.push_back("Arrow");

    _weaponName = WEAPON_LIST[_weaponIndex];

    _armature = BattleParent::getInstance()->factory.buildArmature("Stickman");
    _armatureDisplay = (dragonBones::CCArmatureDisplay *) _armature->getDisplay();

    _armature->getAnimation().fadeIn(Variables::STICKMAN_SETUP_ANIMATION);


    for (auto child : _armatureDisplay->getChildren()) {
        auto bone = _armature->getBoneByDisplay(child);
        if (bone == nullptr) {
            continue;
        }
        if (bone->name == "head") {
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


    _shoulders = _armature->getSlot("Hands")->getChildArmature();
    _shouldersDisplay = (dragonBones::CCArmatureDisplay *) _shoulders->getDisplay();


    const auto firePointBone = _armature->getBone("shoulders");
    cocos2d::Vec2 globalPoint;
    globalPoint.set(firePointBone->global.x, -firePointBone->global.y);
    _shouldersDisplay->setPosition(globalPoint);
    _shouldersDisplay->setAnchorPoint(globalPoint);

    _armature->removeSlot(_armature->getSlot("Apple"));

    _armature->removeBone(_armature->getBone("shoulders"));

    _aimPowerState =  _shoulders->getAnimation().fadeIn(Variables::STICKMAN_SETUP_ANIMATION);
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

    _aimPowerState =  _shoulders->getAnimation().fadeIn(Variables::AIM_IDLE_ANIMATION);

    _string = cocos2d::Node::create();

    _setPlayerView();

    _arrowDisplay->setVisible(false);

    dragonBones::WorldClock::clock.add(_armature);
    this->addChild(_armatureDisplay);
    this->setPosition(_x_pos, _y_pos);
    this->setScale(BattleParent::getInstance()->getGlobalScale());
    BattleParent::getInstance()->addChild(this);
    BattleParent::getInstance()->addStickman();

}

Hero::~Hero() {
}

void Hero::update() {
    _updatePosition();
}

void Hero::attack() {
    if(_faceDir > 0)
        attack(_aim->get_aimRadian(), _aim->get_aimPower());
    else
        attack((180 * dragonBones::ANGLE_TO_RADIAN) - _aim->get_aimRadian(), _aim->get_aimPower());
}

void Hero::aim() {
    _aim->set_aiming(true);
    _aimPowerState = _shoulders->getAnimation().fadeIn(
            Variables::AIM_ANIMATION, 0.f, 1,
            0, "aim", dragonBones::AnimationFadeOutMode::SameGroup
    );
    _arrowDisplay->setVisible(true);
    auto hero_pos = BattleParent::getInstance()->getPlayerPos();

    const auto firePointBone = _shoulders->getSlot("Bow");
    auto globalPoint = cocos2d::Vec2(this->getPosition().x, Variables::translatePoint(
            cocos2d::Vec3(firePointBone->global.x, -firePointBone->global.y, 0.f),
            _shouldersDisplay).y);

    while (!_aimRandomly(globalPoint, hero_pos)) {}

    _updateAim();
}

bool Hero::_aimRandomly(Vec2 start, Vec2 destination) {
    auto power = RandomHelper::random_real(BattleParent::MAX_ARROW_POWER - 5.f, BattleParent::MAX_ARROW_POWER);
    auto x = destination.x - start.x + destination.y - BattleParent::getInstance()->GROUND;
    auto radian = (x + 20.f * power) / 90.f;
    if (radian < 0) {
        radian = (-180 - radian);
        _aim->set_aimRadian(radian * dragonBones::ANGLE_TO_RADIAN);
        _aim->set_aimPower(power);
        return true;
    }
    if (radian > 180) {
        return false;
    }

}


void Hero::attack(float radian, float power) {
    if (_isAttacking) {
        return;
    }
    _isAttacking = true;
    const auto firePointBone = _shoulders->getSlot("Bow");
    auto globalPoint = Variables::translatePoint(cocos2d::Vec3(firePointBone->global.x, -firePointBone->global.y, 0.f),
                                                 _shouldersDisplay);
    globalPoint.x = getPosition().x;

    attack(radian, power, (int)globalPoint.x, (int)globalPoint.y);
}

void Hero::attack(float radian, float power, int x, int y) {
    auto globalPoint = Vec2(x, y);
    int id = getPlayer()->getId();
    switch (_weaponIndex) {
        case 0:
            _fire(new Arrow(_weaponName, radian, power, globalPoint, id));
            break;
        case 1:
            _fire(new Arrow(_weaponName, radian + 3.f * dragonBones::ANGLE_TO_RADIAN, power, globalPoint,
                            id));
            _fire(new Arrow(_weaponName, radian - 3.f * dragonBones::ANGLE_TO_RADIAN, power, globalPoint,
                            id));
            break;

        case 2:
            _fire(new Arrow(_weaponName, radian + 6.f * dragonBones::ANGLE_TO_RADIAN, power, globalPoint,
                            id));
            _fire(new Arrow(_weaponName, radian, power, globalPoint, id));
            _fire(new Arrow(_weaponName, radian - 6.f * dragonBones::ANGLE_TO_RADIAN, power, globalPoint,
                            id));
            break;
        case 3:
            _fire(new PowerArrow(_weaponName, radian, power, globalPoint, id));
            break;
        case 4:
            _fire(new FrozenArrow(_weaponName, radian, power, globalPoint, id));
            break;
        case 5:
            _fire(new FireArrow(_weaponName, radian, power, globalPoint, id));
            break;
        case 6:
            _fire(new BombArrow(_weaponName, radian, power, globalPoint, id));
            break;
        case 7:
            _fire(new MineArrow(_weaponName, radian, power, globalPoint, id));
            break;
        case 8:
            _fire(new DuelArrow(_weaponName, radian, power, globalPoint, id));
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
    setWeapon(_weaponIndex + dest);
}


void Hero::setWeapon(int index) {

    _weaponIndex = index;

    if (_weaponIndex >= WEAPON_LIST.size()) {
        _weaponIndex = 0;
    } else if (_weaponIndex <= 0)
        _weaponIndex = (unsigned int) WEAPON_LIST.size() - 1;

    _shoulders->getSlot("Arrow")->setChildArmature(
            BattleParent::getInstance()->factory.buildArmature(_weaponName));
    _arrowArmature = _shoulders->getSlot("Arrow")->getChildArmature();
    _arrowDisplay = (dragonBones::CCArmatureDisplay *) _arrowArmature->getDisplay();
}

void Hero::_fire(Arrow *arrow) {

    if(auto player = dynamic_cast<PlayerDuel *>(getPlayer())){
        player->addShotsCount();
    }

    _saveAim();

    BattleParent::getInstance()->getBulletPull()->addChild(arrow);

    _arrowDisplay->setVisible(false);
    _state = IDLE;
    _aimPowerState = _shoulders->getAnimation().fadeIn(
            Variables::AIM_IDLE_ANIMATION, 0.f, 1,
            0, "aim", dragonBones::AnimationFadeOutMode::All
    );
    _shouldersDisplay->setRotation(0.f);

    _updateString();
    // setFaceDir();
}

void Hero::_updateAim() {

    if (!_aim->is_aiming()) {
        return;
    }


    _aimPowerState = _shoulders->getAnimation().gotoAndStopByTime(Variables::AIM_ANIMATION, _aim->get_aimPower() / 50);

    float angle = _aim->get_aimRadian() * dragonBones::RADIAN_TO_ANGLE;

    if (_faceDir < 0)
        angle = 180 - angle;

    _aim->set_aimRadian(angle * dragonBones::ANGLE_TO_RADIAN);
    _shouldersDisplay->setRotation((int) angle);

    _shoulders->invalidUpdate("", true);

    const auto firePointBone = _shoulders->getSlot("Bow");
    auto globalPoint = Variables::translatePoint(cocos2d::Vec3(firePointBone->global.x, -firePointBone->global.y, 0.f),
                                                 _shouldersDisplay, this);
    _aim->set_aimPoint(globalPoint);

    _updateString();
}

void Hero::setAim(float angle, float power) {
    if(!_aim->is_aiming()){
        startAim();
    }
    _aim->set_aimRadian(-angle);
    _aim->set_aimPower(power);
    _updateAim();
}


bool Hero::checkAimDiff(float angle, float power) {
    return  _aim->get_aimRadian() - angle > 0.1f || _aim->get_aimPower() - power > 0.5f;
}

void Hero::startAim() {
    UI::enableArrows(this, false);

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
                        cocos2d::Color4F::GRAY);

        line2->drawLine(cocos2d::Vec2(globalPoint.x, globalPoint.y),
                        cocos2d::Vec2(bottom->global.x, -bottom->global.y),
                        cocos2d::Color4F::GRAY);

        _string->addChild(line1, 1);
        _string->addChild(line2, 1);
    } else {
        auto line = cocos2d::DrawNode::create();
        line->drawLine(cocos2d::Vec2(top->global.x, -top->global.y), cocos2d::Vec2(bottom->global.x, -bottom->global.y),
                       cocos2d::Color4F::GRAY);
        _string->addChild(line, 1);
    }
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
    if (DuelScene2P *scene = dynamic_cast<DuelScene2P *>(BattleParent::getInstance())) {
        if (scene->getHeroPos(scene->getHero(getPlayer()->getId())).x < getPosition().x) {
            facedir = -1;
        }
    }
    changeFacedir(facedir);
}

void Hero::_changeArrow() {
    _weaponName = _playerView->getArrow()->Path();
    _arrowArmature = BattleParent::getInstance()->factory.buildArmature(_weaponName);
    _shoulders->getSlot("Arrow")->setChildArmature(_arrowArmature);
    _arrowDisplay = (dragonBones::CCArmatureDisplay *) _arrowArmature->getDisplay();
}

void Hero::_changeHat() {
    _hat = BattleParent::getInstance()->factory.buildArmature(_playerView->getHat()->Path());
    _armature->getSlot("Hat")->setChildArmature(_hat);
}

void Hero::_changeBow() {

    _bowArmature = BattleParent::getInstance()->factory.buildArmature(_playerView->getBow()->Path());

    _shoulders->getSlot("Bow")->setChildArmature(_bowArmature);

    _bowArmatureDisplay = (dragonBones::CCArmatureDisplay *) _bowArmature->getDisplay();

    _string->retain();
    _string->removeFromParent();
    _bowArmatureDisplay->addChild(_string);
    _string->release();

    _updateString();
}


DuelMPHero::DuelMPHero(float x_pos, float y_pos, SocketClient* client) : MPHero(x_pos, y_pos, client) {
    _weaponIndex = 8;
    WEAPON_LIST.push_back("Arrow");
    _player = PlayerDuel::create(client->getDBPlayer()->getId(),
                                        client->getDBPlayer()->getName(),
                                        0
    );
}

void DuelMPHero::switchWeapon(int i) {
    Hero::switchWeapon(i);
}

void DuelMPHero::move(int dir) {
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

void DuelMPHero::setPlayer(Player *player) {
    if(auto pl = dynamic_cast<PlayerDuel *>(player)){
        this->_player = pl;
    }
}

bool DuelMPHero::getHP() {
    return _player->getHp() > 0;
}

void DuelMPHero::dealDamage(float d, cocos2d::Node* bone) {
    auto name = bone->getPhysicsBody()->getName();
    float factor = Variables::getBoneFactor(name);
    _player->setHp((int)(d * factor));
}

Player *DuelMPHero::getPlayer() {
    return _player;
}


DuelHero::DuelHero(float x_pos, float y_pos, PlayerDuel* player) : Hero(x_pos, y_pos) {
    _player = player;
    _weaponIndex = 8;
    WEAPON_LIST.push_back("Arrow");
    setWeapon(_weaponIndex);
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

void DuelHero::setPlayer(Player *player) {
    if(auto pl = dynamic_cast<PlayerDuel *>(player)){
        if(this->_player != nullptr){
            this->removeFromParent();
        }
        this->_player = pl;
    }
}

bool DuelHero::getHP() {
    return _player->getHp() > 0;
}

void DuelHero::dealDamage(float d, cocos2d::Node* bone) {
    auto name = bone->getPhysicsBody()->getName();
    float factor = Variables::getBoneFactor(name);
    _player->setHp((int)(d * factor));

}

Player *DuelHero::getPlayer() {
    return _player;
}

AppleHero::AppleHero(float x_pos, float y_pos, PlayerApple* player) : Hero(x_pos, y_pos) {
    _player = player;
    _weaponIndex = 0;
    WEAPON_LIST.push_back("Arrow");
}

void AppleHero::_saveAim() {
    _aim->set_aiming(false);
}

void AppleHero::setPlayer(Player *player) {
    if(auto pl = dynamic_cast<PlayerApple *>(player)){
        if(this->_player != nullptr){
            this->removeFromParent();
        }
        this->_player = pl;

    }
}

bool AppleHero::getHP() {
    return true;
}

void AppleHero::dealDamage(float d, cocos2d::Node* bone) {
    auto name = bone->getPhysicsBody()->getName();
    float factor = Variables::getBoneFactor(name);
    _player->setHp((int)(d * factor));
}

Player *AppleHero::getPlayer() {
    return _player;
}

MPHero::MPHero(float x_pos, float y_pos, SocketClient* client) : Hero(x_pos, y_pos) {
    _client = client;
    _player = PlayerOnlineApple::create(client->getDBPlayer()->getId(),
                                        client->getDBPlayer()->getName(),
                                        0
    );
}


void MPHero::attack(float angle, float power, int x, int y) {
    Hero::attack(angle, power, x, y);
    _client->action(angle, power, x, y);
}

void MPHero::setPlayer(Player *player) {
    if(auto pl = dynamic_cast<PlayerOnlineApple *>(player)){
        this->_player->removeFromParent();
        this->_player = pl;
    }
}

bool MPHero::getHP() {
    return true;
}

void MPHero::dealDamage(float d, cocos2d::Node* bone) {
    auto name = bone->getPhysicsBody()->getName();
    float factor = Variables::getBoneFactor(name);
    _player->setHp((int)(d * factor));
}

Player *MPHero::getPlayer() {
    return _player;
}
