//
// Created by igor on 04.10.16.
//

#ifndef ARCUNLIM_MAINMENU_H
#define ARCUNLIM_MAINMENU_H

#include "cocos2d.h"

class MainMenu : public cocos2d::Layer
{
public:
    CREATE_FUNC(MainMenu);

    MainMenu();
    ~MainMenu();

    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;

    void onPushScene(int id);
    void onQuit(cocos2d::Ref* sender);

    static cocos2d::Scene *createScene();

    void onChangeLayer(int id);
} ;

class MultiplayerMainMenu : public cocos2d::Layer
{
public:
    CREATE_FUNC(MultiplayerMainMenu);

    MultiplayerMainMenu();
    ~MultiplayerMainMenu();

    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;

    void onPushScene(int id);
    void onQuit(cocos2d::Ref* sender);

} ;

#endif //ARCUNLIM_MAINMENU_H
