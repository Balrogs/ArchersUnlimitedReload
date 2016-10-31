//
// Created by igor on 10.10.16.
//

#ifndef ARCUNLIM_UI_H
#define ARCUNLIM_UI_H

#include "cocos2d.h"

class UI : public cocos2d::LayerColor {
public:
    UI();

    ~UI();

    bool init();

    CREATE_FUNC(UI);

    void initDuel(cocos2d::Size visibleSize, Hero *player);
    void initApple(cocos2d::Size visibleSize, Hero *player);

    void initBattle(cocos2d::Size visibleSize, Hero *player);
};


#endif //ARCUNLIM_UI_H
