#ifndef ARCUNLIM_RANDOMIZER_H
#define ARCUNLIM_RANDOMIZER_H

#include "cocos2d.h"
#include "GameEngine/Objects/Environment/Gift.h"
USING_NS_CC;

class Randomizer : public Layer {
public:
    CREATE_FUNC(Randomizer);

    bool init();

protected:

    bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event);

    bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event);

    Gift* _gift;

    Size _visibleSize;

    void onEnter() override;

    void onQuit();
};


#endif //ARCUNLIM_RANDOMIZER_H
