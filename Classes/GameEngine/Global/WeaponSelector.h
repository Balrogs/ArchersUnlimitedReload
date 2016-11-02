//
// Created by igor on 08.09.16.
//

#ifndef ARCUNLIM_WEAPONSELECTOR_H
#define ARCUNLIM_WEAPONSELECTOR_H

#include <GameEngine/Objects/Hero.h>
#include "cocos2d.h"

class WeaponSelector : public cocos2d::Node {
private:
    cocos2d::Size _SIZE;

    Hero *_hero;

    cocos2d::Rect *_box;

    void _goNext(int dest);

    bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event);

    bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event);

public:

    WeaponSelector(Hero *hero);

    ~WeaponSelector();
};

#endif //ARCUNLIM_WEAPONSELECTOR_H
