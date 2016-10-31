//
// Created by igor on 09.09.16.
//

#ifndef ARCUNLIM_TARGET_H
#define ARCUNLIM_TARGET_H

#include <dragonBones/cocos2dx/CCArmatureDisplay.h>
#include "cocos2d.h"
#include "DragonObject.h"

class Target : public DragonObject {
public:
    float getHP();
    void dealDamage(float hit);
    Target();

protected:
    float _hp;
};

#endif //ARCUNLIM_TARGET_H
