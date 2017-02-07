//
// Created by igor on 03.02.17.
//

#ifndef ARCHERSUNLIMITED_LOBBY_H
#define ARCHERSUNLIMITED_LOBBY_H


#include <GameEngine/Player.h>
#include "MultiplayerMenu.h"
#include "cocos2d.h"

class Lobby : public MultiplayerMenu {
public:
    CREATE_FUNC(Lobby);

    bool init();

    void receiveInvite(string message);

    void deleteInvite();

    void joinLobby();

    void receivePlayerInfo(string message);

    void receiveGlobalStats(string message);

    void receiveCountryStats(string message);

protected:

    Player *_player2;

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



#endif //ARCHERSUNLIMITED_LOBBY_H
