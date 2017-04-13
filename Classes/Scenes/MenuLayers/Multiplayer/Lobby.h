//
// Created by igor on 03.02.17.
//

#ifndef ARCHERSUNLIMITED_LOBBY_H
#define ARCHERSUNLIMITED_LOBBY_H


#include <GameEngine/Player.h>
#include <ui/UIScrollView.h>
#include <GameEngine/Global/Misc/UI/RichSearchButton.h>
#include "MultiplayerMenu.h"
#include "GameEngine/Global/Misc/UI/PopUp.h"
#include "cocos2d.h"

class Lobby : public cocos2d::Layer {
public:
    CREATE_FUNC(Lobby);

    bool init();

    void receiveInvite(string message);

    void acceptInvite();

    void denyInvite();

    void leaveLobby();

    void deleteInvite();

    void startSearch(int gameType);

    void joinLobby();

    void receivePlayerInfo(string message);

    void receiveGlobalStats(string message);

    void receiveCountryStats(string message);

    void addFriend(string name);

    void showSearchPopUp();

    virtual void onError(string message);

    virtual void onQuit();

protected:
    void _showPopUp(PopUp* popUp);

    int _player2Id;
    std::string _player2Name;

    void onEnter() override;

    void _resetPlayer();

    cocos2d::Label *_errorMessage;
    SocketClient *_client;

    cocos2d::Size _visibleSize;


private:

    cocos2d::Node *_leftPart1;
    cocos2d::Node *_leftPart2;
    cocos2d::Node *_rightPart1;
    cocos2d::Node *_rightPart2;

    int _gameType;

    RichSearchButton* _findPlayerButton;
    cocos2d::Node *_friendsBox;
    cocos2d::ui::ScrollView *_scrollView;
    cocos2d::ui::Button *_findFriendButton;

    cocos2d::Node *_playerInfo;
    cocos2d::Node *_playerGlobalStatistics;
    cocos2d::Node *_playerCountryStatistics;

    cocos2d::ui::Button *_playerInfoButton;
    cocos2d::ui::Button *_playerGlobalStatisticsButton;
    cocos2d::ui::Button *_playerCountryStatisticsButton;

    cocos2d::Node *_playerInfoBox;
    cocos2d::Node *_playerGlobalStatisticsBox;
    cocos2d::Node *_playerCountryStatisticsBox;

    void _setSearchButtonState();

    void _showScrollView();

    void _reloadInfoBox(int type);
};



#endif //ARCHERSUNLIMITED_LOBBY_H
