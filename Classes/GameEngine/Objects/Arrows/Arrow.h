#ifndef DRAGONBONES_ARROW_H
#define DRAGONBONES_ARROW_H

#include <GameEngine/Objects/DragonObject.h>
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "cocos2d.h"

class Arrow : public DragonObject {
public:
    Arrow(const std::string &armatureName, float radian, float power, const cocos2d::Vec2 &position,
              int player_id);

    virtual bool processContact(cocos2d::Node *bone);

    virtual void addToNode(cocos2d::Node *bone);

    virtual void update(float dt);

    ~Arrow();

protected:
    float _speedX;
    float _damage;
    float _offset;
    float _speedY;
    bool _isActive;
    int _player_id;
    float _x_limit;

    cocos2d::Vec2 _head;
    cocos2d::Vec2 _tail;

    void _updateAnimation() override;
    void update() override;

    virtual void _disableArrow();
    virtual void afterAction();

    void addToBox(cocos2d::Node *bone);

};



#endif //DRAGONBONES_ARROW_H
