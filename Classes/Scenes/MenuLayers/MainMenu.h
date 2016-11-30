//
// Created by igor on 04.10.16.
//

#ifndef ARCUNLIM_MAINMENU_H
#define ARCUNLIM_MAINMENU_H

#include <GameEngine/Global/Misc/Views.h>
#include "cocos2d.h"
#include <ui/UIEditBox/UIEditBox.h>
#include <GameEngine/Player.h>

class MainMenu : public cocos2d::Layer
{
public:
    CREATE_FUNC(MainMenu);

    MainMenu();
    ~MainMenu();
    void onEnter() override;
    void onPushScene(int id);
    void onChangeLayer();
    void onQuit();

    static cocos2d::Scene *createScene();
} ;

class MultiplayerMainMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene *createScene();
    static MultiplayerMainMenu* getInstance();
    CREATE_FUNC(MultiplayerMainMenu);

    virtual void onError(string message);
    virtual void onQuit();

protected:
    static MultiplayerMainMenu *_instance;
    MultiplayerMainMenu();
    void onEnter();
    void onPushScene(int id);
    void update(float dt);

    cocos2d::Label *_errorMessage;
    SocketClient *_client;

private:
    cocos2d::ui::EditBox *_editName;
    cocos2d::ui::EditBox *_editPassword;
};

class RegisterMenu : public cocos2d::Layer
{
public:

    RegisterMenu();
    void onPushScene(int id) ;
    void onQuit();

protected:
    cocos2d::ui::EditBox *_editName;
    cocos2d::ui::EditBox *_editPassword;
    cocos2d::Label *_errorMessage;
    SocketClient *_client;

};


class LobbyLayer : public MultiplayerMainMenu
{
public:

    static cocos2d::Scene *createScene();
    static LobbyLayer* getInstance();
    CREATE_FUNC(LobbyLayer);


    void receiveInvite(string message);
    void deleteInvite();
    void receivePlayerInfo(string message);
    void receiveGlobalStats(string message);
    void receiveCountryStats(string message);

protected:

    static LobbyLayer *_instance;
    Player* _player2;
    LobbyLayer();
    void onEnter() override;

private:

    cocos2d::ui::Button *_acceptButton;
    cocos2d::ui::Button *_denyButton;
    cocos2d::ui::Button *_inviteButton;
    cocos2d::ui::Button *_findPlayerButton;
    cocos2d::ui::Button *_findFriendButton;
    cocos2d::Node *_inviteBox;
    cocos2d::Node *_moreInfoBox;
    cocos2d::Node *_playerInfoBox;
    cocos2d::Node *_playerGlobalStatisticsBox;
    cocos2d::Node *_playerCountryStatisticsBox;
};


#endif //ARCUNLIM_MAINMENU_H
