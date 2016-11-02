#ifndef ARCUNLIM_APPLE_H
#define ARCUNLIM_APPLE_H

#include <GameEngine/Objects/Target.h>

class Apple : public cocos2d::Node {

public:
    Apple(float x_pos, float y_pos);

    ~Apple();

    void hit();

};

#endif //ARCUNLIM_APPLE_H
