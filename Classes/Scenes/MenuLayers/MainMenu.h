//
// Created by igor on 04.10.16.
//

#ifndef ARCUNLIM_MAINMENU_H
#define ARCUNLIM_MAINMENU_H

#include <GameEngine/Global/Misc/SocketClient.h>
#include "cocos2d.h"
#include <ui/UIEditBox/UIEditBox.h>

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

    MultiplayerMainMenu();
    MultiplayerMainMenu(SocketClient *client);

    void onPushScene(int id);
    void onQuit(cocos2d::Ref* sender);

protected:
    SocketClient *_client;

};

class RegisterMenu : public cocos2d::Layer
{
public:

    RegisterMenu(SocketClient *client);

    void onPushScene(int id) ;
    void onQuit(cocos2d::Ref* sender);

protected:
    cocos2d::ui::EditBox *_editName;
    cocos2d::ui::EditBox *_editPassword;
    cocos2d::Label *_errorMessage;
    SocketClient *_client;

};

class LoginLayer : public cocos2d::Layer
{
public:

    LoginLayer(SocketClient *client);

    void onPushScene(int id) ;
    void onQuit(cocos2d::Ref* sender);

protected:
    cocos2d::ui::EditBox *_editPassword;
    cocos2d::Label *_errorMessage;
    SocketClient *_client;

};

class LobbyLayer : public cocos2d::Layer
{
public:

    LobbyLayer(SocketClient *client);

    void onQuit(cocos2d::Ref* sender);

protected:


    void onEnter() override;

    void wait(float dt);
    cocos2d::ui::Button *_acceptButton;
    cocos2d::Label *_inviteMessage;
    cocos2d::Label *_errorMessage;
    SocketClient *_client;

};


#endif //ARCUNLIM_MAINMENU_H
