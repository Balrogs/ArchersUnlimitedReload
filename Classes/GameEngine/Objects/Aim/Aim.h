//
// Created by igor on 08.09.16.
//

#ifndef ARCUNLIM_AIM_H
#define ARCUNLIM_AIM_H

#include <cocos2d.h>
#include "AimPreview.h"

class Aim : public cocos2d::Node {

private:
    cocos2d::Vec2 _aimPoint;
    bool _aiming;
    float _aimRadian;
    float _aimPower;
    float _aimPreviewCount;

    void _update();

    float setScale(int i);

    cocos2d::Vec2 setPosition(int i);

public:

    float get_aimPower() const;

    bool is_aiming() const;

    float get_aimRadian() const;

    void set_aimPoint(cocos2d::Vec2 _aimPoint);

    void set_aiming(bool _aiming);

    void set_aimRadian(float _aimRadian);

    void set_aimPower(float _aimPower);

    void disable();

    Aim();

    ~Aim();

    void _addAimPreview(float scale, const cocos2d::Vec2 &position, cocos2d::Color4F color);
};


#endif //ARCUNLIM_AIM_H
