#ifndef ARCHERSUNLIMITED_FIREARROW_H
#define ARCHERSUNLIMITED_FIREARROW_H


#include "Arrow.h"

class FireArrow : public Arrow {

public:

    FireArrow(const std::string &armatureName, float radian, float power,
               const cocos2d::Vec2 &position, int player_id);

    void update(float dt) override ;

    bool processContact(cocos2d::Node *bone) override;

    ~FireArrow();
};

#endif //ARCHERSUNLIMITED_FIREARROW_H
