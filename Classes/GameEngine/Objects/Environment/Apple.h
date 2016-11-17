#ifndef ARCUNLIM_APPLE_H
#define ARCUNLIM_APPLE_H

#include "cocos2d.h"

class Apple : public cocos2d::Node {

public:
    Apple(float x_pos, float y_pos);

    ~Apple();

    void hit();

};

#endif //ARCUNLIM_APPLE_H
