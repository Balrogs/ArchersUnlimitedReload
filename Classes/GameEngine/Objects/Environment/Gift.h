#ifndef ARCHERSUNLIMITED_GIFT_H
#define ARCHERSUNLIMITED_GIFT_H

#include "cocos2d.h"

USING_NS_CC;

class Gift : public Node {
public:
    CREATE_FUNC(Gift);
    bool init();
    void breakBox();
    bool checkTouch(Vec2 point);
    void enable();
private:
    bool _isBroken;
    bool _isEnabled;
    Sprite* _box;
};


#endif //ARCHERSUNLIMITED_GIFT_H
