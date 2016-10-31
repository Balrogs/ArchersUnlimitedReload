//
// Created by igor on 08.09.16.
//

#ifndef ARCUNLIM_WEAPONSELECTOR_H
#define ARCUNLIM_WEAPONSELECTOR_H

#include <GameEngine/Objects/Hero.h>
#include "cocos2d.h"

class WeaponSelector : public cocos2d::Node {
private:
    static const float POSITION_X;
    static const float POSITION_Y;
    Hero *_hero;

public:

    WeaponSelector(Hero* hero);

    ~WeaponSelector();
};


#endif //ARCUNLIM_WEAPONSELECTOR_H
