//
// Created by igor on 05.10.16.
//

#ifndef ARCUNLIM_EQUIPMENTSCENE_H
#define ARCUNLIM_EQUIPMENTSCENE_H

#include <GameEngine/Objects/Hero.h>
#include "cocos2d.h"

class EquipmentScene : public cocos2d::Layer {
public:
    CREATE_FUNC(EquipmentScene);

    bool init();
    cocos2d::Vec2 getButtonPosition();
protected:
    cocos2d::Size _visibleSize;

    HeroPreview* _hero;

    void onEnter() override;
    void onQuit();
};
#endif //ARCUNLIM_EQUIPMENTSCENE_H
