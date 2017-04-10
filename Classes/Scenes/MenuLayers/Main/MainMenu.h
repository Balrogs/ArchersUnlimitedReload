//
// Created by igor on 04.10.16.
//

#ifndef ARCUNLIM_MAINMENU_H
#define ARCUNLIM_MAINMENU_H

#include <GameEngine/Global/Misc/Views.h>
#include "cocos2d.h"
#include "EquipmentScene.h"
#include "Scenes/Layers/BackgroundLayer.h"
#include <ui/UIEditBox/UIEditBox.h>
#include <GameEngine/Player.h>


class MainScene : public cocos2d::Scene{
public:
    static MainScene* getInstance();

    static MainScene *create();

    bool init();

    void replaceMain(cocos2d::Layer* layer);

    void pushMain(cocos2d::Layer* layer);

    void wait(bool p);

    void popMain();

    void popAndReplace();

    cocos2d::Layer *getMain();

private:

    static MainScene *_instance;

    EquipmentScene *_equipmentScene;
    cocos2d::Layer *_main;
    BackgroundLayer *_backgroundLayer;

    std::stack<cocos2d::Layer *> _mainStack;
};

class MainMenu : public cocos2d::Layer {
public:
    static MainMenu *create(EquipmentScene* equipmentLayer);

    bool init(EquipmentScene* equipmentLayer);

    void onEnter() override;

    void onPushScene(int id);

    void showErrorPopUp();

private:

    void _enterFrameHandler(float passedTime);

    cocos2d::Size _visibleSize;
    int _menuId;
    cocos2d::Label* _coinsCount;
    cocos2d::Node* _menu;
    void onMenuClick(int id);
};

#endif //ARCUNLIM_MAINMENU_H
