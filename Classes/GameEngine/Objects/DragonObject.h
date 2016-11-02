//
// Created by igor on 17.10.16.
//

#ifndef ARCUNLIM_DRAGONOBJECT_H
#define ARCUNLIM_DRAGONOBJECT_H

#include <dragonBones/cocos2dx/CCArmatureDisplay.h>
#include "cocos2d.h"

class DragonObject : public cocos2d::Sprite {

public:

    static DragonObject *create();

    dragonBones::CCArmatureDisplay *getDisplay();

    cocos2d::Node *getNodeByName(std::string name);

    float getGlobalHeight(std::string name);

    void addDOChild(cocos2d::Node* target);

    virtual void update();

    DragonObject();

protected:
    dragonBones::Armature *_armature;
    dragonBones::CCArmatureDisplay *_armatureDisplay;
    float _x_pos;
    float _y_pos;

    virtual void _updateAnimation();
};


#endif //ARCUNLIM_DRAGONOBJECT_H
