//
// Created by igor on 08.09.16.
//

#ifndef DRAGONBONES_HERO_H
#define DRAGONBONES_HERO_H

#include <GameEngine/Objects/Arrows/Arrow.h>
#include <GameEngine/Objects/Aim/Aim.h>
#include <GameEngine/Global/Misc/SocketClient.h>
#include "cocos2d.h"
#include "Stickman.h"

class Hero : public Body {

public:

    Hero(float x_pos, float y_pos);

    ~Hero();

    void attack(float angle, float power);

    virtual void attack(float angle, float power, int x, int y);

    void setAim(float angle, float power);

    bool checkAimDiff(float angle, float power);

    virtual void switchWeapon(int dest);

    virtual void dealDamage(float d, cocos2d::Node* bone) = 0;

    void setWeapon(unsigned int index);

    void update() override;

    void startAim();

    void attack() override;

    void aim(float factor) override;

    virtual void setPlayer(Player *player) = 0;

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
    dragonBones::Armature *_hat;
    dragonBones::CCArmatureDisplay *_bowArmatureDisplay;
    cocos2d::Node *_string;

    void _fire(Arrow *arrow);

    void _updateAnimation() override;

    void _updateAim();

    void _updateString();

    virtual void _saveAim();

    void setFaceDir();

    bool _aimRandomly(cocos2d::Vec2 start, cocos2d::Vec2 destination, float factor);

    void _changeArrow();

    void _changeHat();

    void _changeBow();
};

class AppleHero : public Hero {
public:
    AppleHero(float x_pos, float y_pos, PlayerApple* player);
    void _saveAim() override;

    virtual void setPlayer(Player *player);

    virtual bool getHP();

    virtual void dealDamage(float d, cocos2d::Node* bone);

    virtual Player *getPlayer();

protected:
    PlayerApple* _player;
};

class MPHero : public Hero {
public:
    MPHero(float x_pos, float y_pos, SocketClient* client);
    void attack(float angle, float power, int x, int y) override;

    virtual void setPlayer(Player *player) ;

    virtual bool getHP();

    virtual void dealDamage(float d, cocos2d::Node* bone);

    virtual Player *getPlayer();
protected:
    SocketClient* _client;
    PlayerOnlineApple* _player;
};

class DuelMPHero : public MPHero {
public:

    DuelMPHero(float x_pos, float y_pos, SocketClient* client);

    void switchWeapon(int i) override;

    void move(int dir) override;

    virtual void setPlayer(Player *player);

    virtual bool getHP();

    virtual void dealDamage(float d, cocos2d::Node* bone);

    virtual Player *getPlayer();

protected:
    PlayerDuel* _player;
};

class DuelHero : public Hero {
public:

    DuelHero(float x_pos, float y_pos, PlayerDuel* player);

    void switchWeapon(int i) override;

    void move(int dir) override;

    virtual void setPlayer(Player *player);

    virtual bool getHP();

    virtual void dealDamage(float d, cocos2d::Node* bone);

    virtual Player *getPlayer();

protected:
    PlayerDuel* _player;
};




#endif //DRAGONBONES_HERO_H
