#ifndef ARCUNLIM_RANDOMIZER_H
#define ARCUNLIM_RANDOMIZER_H

#include "cocos2d.h"
USING_NS_CC;

class Randomizer : public Layer {
public:
    CREATE_FUNC(Randomizer);

    bool init();

protected:

    Size _visibleSize;

    void onEnter() override;

    void onQuit();
};


#endif //ARCUNLIM_RANDOMIZER_H
