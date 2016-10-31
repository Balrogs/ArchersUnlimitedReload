//
// Created by igor on 08.09.16.
//

#ifndef DRAGONBONES_ARROW_H
#define DRAGONBONES_ARROW_H

#include <GameEngine/Objects/Target.h>
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "cocos2d.h"

class Arrow : public DragonObject {
protected:
    float _speedX;
    float _offset;
    float _speedY;
    bool _isActive;
    int _player_id;
    float _x_limit;
    cocos2d::Vec2 _head;
    cocos2d::Vec2 _tail;

    void _updateAnimation() override ;
    void update() override ;

public:
    Arrow(const std::string &armatureName, float radian, float power, const cocos2d::Vec2 &position,
              int player_id);
    virtual bool processContact(cocos2d::Node *bone);
    virtual void addToNode(cocos2d::Node *bone);
    ~Arrow();
    virtual void update(float dt);

protected:
    virtual void _disableArrow();
    virtual void afterAction();

};


class PowerArrow : public Arrow {

public:

    PowerArrow(const std::string &armatureName, float radian, float power,
               const cocos2d::Vec2 &position, int player_id);

    bool processContact(cocos2d::Node *bone) override;

    ~PowerArrow();
};

class FrozenArrow : public Arrow {

public:

    FrozenArrow(const std::string &armatureName, float radian, float power,
               const cocos2d::Vec2 &position, int player_id);

    void update(float dt) override ;

    bool processContact(cocos2d::Node *bone) override;

    ~FrozenArrow();
};

class FireArrow : public Arrow {

public:

    FireArrow(const std::string &armatureName, float radian, float power,
               const cocos2d::Vec2 &position, int player_id);

    void update(float dt) override ;

    bool processContact(cocos2d::Node *bone) override;

    ~FireArrow();
};

class BombArrow : public Arrow {

public:

    BombArrow(const std::string &armatureName, float radian, float power,
              const cocos2d::Vec2 &position, int player_id);

    bool processContact(cocos2d::Node *bone) override;

protected:
    void afterAction() override ;
    void _disableArrow() override;
};

class MineArrow : public Arrow {

public:

    void update(float dt) override ;

    MineArrow(const std::string &armatureName, float radian, float power,
              const cocos2d::Vec2 &position, int player_id);

    bool processContact(cocos2d::Node *bone) override;

    ~MineArrow();
};

class DuelArrow : public Arrow {

public:

    void update(float dt) override ;

    DuelArrow(const std::string &armatureName, float radian, float power,
              const cocos2d::Vec2 &position, int player_id);

    bool processContact(cocos2d::Node *bone) override;

    ~DuelArrow();
};


#endif //DRAGONBONES_ARROW_H
