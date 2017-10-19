#ifndef ARCHERSUNLIMITED_DUELARROW_H
#define ARCHERSUNLIMITED_DUELARROW_H


#include "Arrow.h"

class DuelArrow : public Arrow {

public:

    void update(float dt) override ;

    DuelArrow(const std::string &armatureName, float radian, float power,
              const cocos2d::Vec2 &position, int player_id);

    bool processContact(cocos2d::Node *bone) override;

    ~DuelArrow();

    int lifePeriod;
};



#endif //ARCHERSUNLIMITED_DUELARROW_H
