//
// Created by igor on 03.02.17.
//

#ifndef ARCHERSUNLIMITED_LOBBY_H
#define ARCHERSUNLIMITED_LOBBY_H


#include <GameEngine/Player.h>
#include <ui/UIScrollView.h>
#include "MultiplayerMenu.h"
#include "GameEngine/Global/Misc/PopUp.h"
#include "cocos2d.h"

class Lobby : public MultiplayerMenu {
public:
    CREATE_FUNC(Lobby);

    bool init();

    void receiveInvite(string message);

    void acceptInvite();

    void denyInvite();

    void deleteInvite();

    void startSearch(int gameType);

    void joinLobby();

    void receivePlayerInfo(string message);

    void receiveGlobalStats(string message);

    void receiveCountryStats(string message);

    void addFriend(string name);

protected:
    void _showPopUp(PopUp* popUp);

    Player *_player2;


private:

    int _gameType;

    cocos2d::ui::Button *_findPlayerButton;
    cocos2d::Node *_loading;
    cocos2d::Node *_inviteBox;

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

    void _reloadInviteBox(bool isEmpty);

    void _showScrollView();

    void _reloadInfoBox(int type);
};



#endif //ARCHERSUNLIMITED_LOBBY_H
