//
// Created by igor on 08.09.16.
//

#ifndef DRAGONBONES_HERO_H
#define DRAGONBONES_HERO_H

#include <GameEngine/Objects/Arrows/Arrow.h>
#include <GameEngine/Player.h>
#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "GameEngine/Objects/Aim/Aim.h"
#include "Stickman.h"

class Hero : public Body {

public:

    Hero(float x_pos, float y_pos, Player *player);

    ~Hero();

    void attack(float angle, float power, float x, float y);

    void setAim(float angle, float power);

    virtual void switchWeapon();

    void update() override;

    void startAim();

    void attack() override;

    void aim() override;

    Player *getPlayer() const;

protected:
    bool _isAttacking;
    unsigned _weaponIndex;
    std::string _weaponName;
    std::vector<std::string> WEAPON_LIST;
    Aim *_aim;
    Player *_player;
    dragonBones::AnimationState *_aimPowerState;
    dragonBones::Armature *_shoulders;
    dragonBones::Armature *_arrowArmature;
    dragonBones::CCArmatureDisplay *_arrowDisplay;
    dragonBones::CCArmatureDisplay *_shouldersDisplay;
    dragonBones::Armature *_bowArmature;
    dragonBones::CCArmatureDisplay *_bowArmatureDisplay;
    cocos2d::Node *_string;

    void _fire(Arrow *arrow);

    void _updateAnimation() override;

    void _updateAim();

    void _updateString();
};


class DuelHero : public Hero {
public:
    DuelHero(float x_pos, float y_pos);

    DuelHero(float x_pos, float y_pos, Player *player);

    void switchWeapon() override;
};

#endif //DRAGONBONES_HERO_H
