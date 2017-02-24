//
// Created by igor on 08.09.16.
//

#ifndef DRAGONBONES_STICKMAN_H
#define DRAGONBONES_STICKMAN_H

#include <GameEngine/Player.h>
#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "DragonObject.h"


enum State {
    JUMPING, SITTING, ATTACKING, IDLE, MOVING, HITTED
};

class Body : public DragonObject {

protected:
    State _state;
    int _faceDir;
    int _moveDir;
    float _speedX;
    float _speedY;
    float _move_speed;
    float _jump_speed;

public:

    Body(float x_pos, float y_pos, float scale, float facedir);

    void setSpeed(float move_speed);

    void setJumpSpeed(float jump_speed);

    virtual void move(int dir);

    void _updatePosition();

    virtual void kill();

    virtual void attack() = 0;

    virtual void aim() = 0;

    State getState();

    void setState(State state);

    virtual void changeFacedir(int facedir);

    virtual bool getHP() = 0;

    virtual void dealDamage(float d) = 0;

    virtual Player *getPlayer() = 0;
};

class Stickman : public Body {


public:
    Stickman(float x_pos, float y_pos, float scale, float hp);

    ~Stickman();

    void jump();

    void sit();

    void hit(cocos2d::Vec2 velocity);

    void attack() override;

    void aim() override;

    void update() override;

    virtual bool getHP();

    virtual void dealDamage(float d);

    Bot *_bot;

    virtual Player *getPlayer();

protected:
    void _updateAnimation() override;
};

#endif //DRAGONBONES_STICKMAN_H
