//
// Created by igor on 05.10.16.
//

#ifndef ARCUNLIM_EQUIPMENTSCENE_H
#define ARCUNLIM_EQUIPMENTSCENE_H

#include "cocos2d.h"

class EquipmentScene : public cocos2d::Layer {
public:
    CREATE_FUNC(EquipmentScene);

    bool init();

protected:

    void onEnter() override;

    void onQuit();

};
#endif //ARCUNLIM_EQUIPMENTSCENE_H
