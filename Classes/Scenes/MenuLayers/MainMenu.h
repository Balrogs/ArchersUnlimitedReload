//
// Created by igor on 04.10.16.
//

#ifndef ARCUNLIM_MAINMENU_H
#define ARCUNLIM_MAINMENU_H

#include <GameEngine/Global/Misc/Views.h>
#include "cocos2d.h"
#include <ui/UIEditBox/UIEditBox.h>

class MainMenu : public cocos2d::Layer
{
public:
    CREATE_FUNC(MainMenu);

    MainMenu();
    ~MainMenu();

    void onPushScene(int id);
    void onChangeLayer(int id);
    void onQuit(cocos2d::Ref* sender);

    static cocos2d::Scene *createScene();
} ;

class MultiplayerMainMenu : public cocos2d::Layer
{
public:

    MultiplayerMainMenu();

    void onEnter();
    void onPushScene(int id);
    void onQuit(cocos2d::Ref* sender);

protected:
    SocketClient *_client;

};

class RegisterMenu : public cocos2d::Layer
{
public:

    RegisterMenu();
    void onExit();
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

    LoginLayer();

    void onPushScene(int id) ;
    void onQuit(cocos2d::Ref* sender);

protected:
    cocos2d::ui::EditBox *_editName;
    cocos2d::ui::EditBox *_editPassword;
    cocos2d::Label *_errorMessage;
    SocketClient *_client;
};

class LobbyLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene *createScene();
    static LobbyLayer* getInstance();
    CREATE_FUNC(LobbyLayer);


    void receiveInvite(string message);
    void receivePlayerInfo(string message);
    void receiveGlobalStats(string message);
    void receiveCountryStats(string message);
    void onQuit(cocos2d::Ref* sender);

protected:
    LobbyLayer();
    void onEnter() override;

    cocos2d::ui::Button *_acceptButton;
    cocos2d::Node *_inviteBox;
    cocos2d::Node *_moreInfoBox;
    cocos2d::Node *_playerInfoBox;
    cocos2d::Node *_playerGlobalStatisticsBox;
    cocos2d::Node *_playerCountryStatisticsBox;
    cocos2d::Label *_errorMessage;
    SocketClient *_client;
    int _opponentId;
};


#endif //ARCUNLIM_MAINMENU_H
