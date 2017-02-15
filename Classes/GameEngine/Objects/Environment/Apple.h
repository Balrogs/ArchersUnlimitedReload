#ifndef ARCUNLIM_APPLE_H
#define ARCUNLIM_APPLE_H

#include "cocos2d.h"

class Apple : public cocos2d::Node {

public:
    Apple(float x_pos, float y_pos, int id);

    ~Apple();

    void hit();

    int getId();

private:
    int _id;
};

#endif //ARCUNLIM_APPLE_H
