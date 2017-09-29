#ifndef ARCUNLIM_EQUIPMENTSCENE_H
#define ARCUNLIM_EQUIPMENTSCENE_H

#include <GameEngine/Objects/Hero.h>
#include <ui/UIButton.h>
#include <GameEngine/Global/Misc/UI/UIControls.h>
#include "cocos2d.h"

USING_NS_CC;

class EquipmentScene : public Layer {
public:

    static EquipmentScene* getInstance();

    CREATE_FUNC(EquipmentScene);

    bool init();

    cocos2d::Vec2 getButtonPosition();

    bool resumeEquipment();

    dragonBones::CCFactory factory;

    bool checkTouch(Vec2 pos);

protected:

    bool _busy;
    bool _paused;

    static EquipmentScene *_instance;

    Size _visibleSize;
    Rect _heroBox;

    UIControls* _controls;
    HeroPreview* _hero;

    Label* _hatsCount;
    Label* _arrowsCount;
    Label* _bowsCount;

    void _readAssets();

    void _enterFrameHandler(float passedTime);

    void onQuit();
};
#endif //ARCUNLIM_EQUIPMENTSCENE_H
