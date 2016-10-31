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
} ;

#endif //ARCUNLIM_MAINMENU_H
