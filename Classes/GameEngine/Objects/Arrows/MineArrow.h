#ifndef ARCHERSUNLIMITED_MINEARROW_H
#define ARCHERSUNLIMITED_MINEARROW_H


#include "Arrow.h"

class MineArrow : public Arrow {

public:

    void update(float dt) override ;

    MineArrow(const std::string &armatureName, float radian, float power,
              const cocos2d::Vec2 &position, int player_id);

    bool processContact(cocos2d::Node *bone) override;

    ~MineArrow();
};

#endif //ARCHERSUNLIMITED_MINEARROW_H
