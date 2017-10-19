
#ifndef ARCHERSUNLIMITED_BOMBARROW_H
#define ARCHERSUNLIMITED_BOMBARROW_H


#include "Arrow.h"

class BombArrow : public Arrow {

public:

    BombArrow(const std::string &armatureName, float radian, float power,
              const cocos2d::Vec2 &position, int player_id);

    bool processContact(cocos2d::Node *bone) override;

protected:
    void afterAction() override ;
    void _disableArrow() override;
};

#endif //ARCHERSUNLIMITED_BOMBARROW_H
