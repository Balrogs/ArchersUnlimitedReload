#ifndef ARCHERSUNLIMITED_FROZENARROW_H
#define ARCHERSUNLIMITED_FROZENARROW_H

#include "Arrow.h"

class FrozenArrow : public Arrow {

public:

    FrozenArrow(const std::string &armatureName, float radian, float power,
               const cocos2d::Vec2 &position, int player_id);

    void update(float dt) override ;

    bool processContact(cocos2d::Node *bone) override;

    ~FrozenArrow();

};


#endif //ARCHERSUNLIMITED_FROZENARROW_H
