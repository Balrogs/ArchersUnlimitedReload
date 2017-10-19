#ifndef ARCHERSUNLIMITED_POWERARROW_H
#define ARCHERSUNLIMITED_POWERARROW_H


#include "Arrow.h"

class PowerArrow : public Arrow {

public:

    PowerArrow(const std::string &armatureName, float radian, float power,
               const cocos2d::Vec2 &position, int player_id);

    bool processContact(cocos2d::Node *bone) override;

    ~PowerArrow();
};


#endif //ARCHERSUNLIMITED_POWERARROW_H
