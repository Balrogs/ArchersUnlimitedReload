
#ifndef ARCUNLIM_BRAIN_H
#define ARCUNLIM_BRAIN_H

#include <GameEngine/Objects/Stickman.h>
#include "cocos2d.h"


class Brain : public cocos2d::Node {
protected:

    float _jump_speed;
    float _move_speed;

    int _current_dir;
    Body *_body;

    int chooseDir();

    virtual bool isTargetNear();

    void remove();

public:
    Brain(Body *body);

    Brain(Body *body, float jump_speed, float move_speed, float upd);

    virtual void update();

    void update(float dt);

    ~Brain();
};

class PassiveBrain : public Brain {
public:
    PassiveBrain(Body *body);

    void update() override;
};

class FastBrain : public Brain {
public:
    FastBrain(Body *body);

    void update() override;
};

class StrengthBrain : public Brain {
public:
    StrengthBrain(Body *body);

    void update() override;
};

class HeroBrain : public Brain {
public:
    HeroBrain(Body *body, float upd);

    void update() override;

    bool isTargetNear() override;

protected:
    virtual void attack();

    virtual void aim();
};


class HeroBrainDuel : public Brain {
public:
    HeroBrainDuel(Body *body, float upd);

    bool isTargetNear() override;

    void update() override;

protected:
    void attack();

    void aim();
};


#endif //ARCUNLIM_BRAIN_H
