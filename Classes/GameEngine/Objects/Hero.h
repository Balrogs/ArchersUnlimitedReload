//
// Created by igor on 08.09.16.
//

#ifndef DRAGONBONES_HERO_H
#define DRAGONBONES_HERO_H

#include <GameEngine/Objects/Arrows/Arrow.h>
#include <GameEngine/Objects/Aim/Aim.h>
#include "cocos2d.h"
#include "Stickman.h"

class Hero : public Body {

public:

    Hero(float x_pos, float y_pos);

    Hero(float x_pos, float y_pos, Player *player);

    ~Hero();

    void attack(float angle, float power);

    void setAim(float angle, float power);

    bool checkAimDiff(float angle, float power);

    virtual void switchWeapon(int dest);

    void update() override;

    void startAim();

    void attack() override;

    void aim() override;

    void setPlayer(Player *player);

    std::vector<std::string> getWeaponList();

protected:
    bool _isAttacking;
    unsigned _weaponIndex;
    std::string _weaponName;
    std::vector<std::string> WEAPON_LIST;
    Aim *_aim;
    Aim *_prevAim;
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

    virtual void _saveAim();

    void setFaceDir();

    bool _aimRandomly(cocos2d::Vec2 start, cocos2d::Vec2 destination);
};


class DuelHero : public Hero {
public:

    DuelHero(float x_pos, float y_pos, Player* player);

    DuelHero(float x_pos, float y_pos, const char *name);

    void switchWeapon(int i) override;

    void move(int dir) override;
};

class AppleHero : public Hero {
public:
    AppleHero(float x_pos, float y_pos, const char *name, int coins);
    void _saveAim() override;
};


#endif //DRAGONBONES_HERO_H
