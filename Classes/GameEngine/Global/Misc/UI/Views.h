//
// Created by igor on 24.11.16.
//

#ifndef ARCHERSUNLIMITED_VIEWS_H
#define ARCHERSUNLIMITED_VIEWS_H

#include "cocos2d.h"
#include "GameEngine/Global/Misc/SocketClient.h"

class RankViewBase : public cocos2d::Node {
public:
    static RankViewBase* create(cocos2d::Size size, cocos2d::Color4B color);
    bool init(cocos2d::Size size, cocos2d::Color4B color);
private:
    cocos2d::Node* _bg;
};

class RankView {
public:
    static const cocos2d::Color4B BG_COLOR;
    static const cocos2d::Color4B PLAYER_COLOR;
    static const cocos2d::Color4B HEADER_COLOR;

    RankView(string name, int country, int rank);

    string getPlayerName();
    int getCountry();
    int getRank();

    static cocos2d::Node* getHeader(cocos2d::Size size);
    cocos2d::Node* getView(cocos2d::Size size, bool isPlayer);

private:
    string _name;
    int _country;
    int _rank;

    static cocos2d::Size _getNameSize(cocos2d::Size size);
    static cocos2d::Size _getRegionSize(cocos2d::Size size);
    static cocos2d::Size _getRankSize(cocos2d::Size size);
};

class RankTable : public cocos2d::Node {
public:
    static RankTable* create(int length, cocos2d::Size size);
    bool init(int length, cocos2d::Size size);
    void addRank(cocos2d::Node* rankView, int id);

    cocos2d::Size getRankSize();

protected:
    int _size;
    cocos2d::Size _rankSize;
    std::vector<RankView*> _rankings;
};


class Views : public cocos2d::Node {
public:

    /*
     * {"s_type":1, "name":"global","code":9}

{"s_type":2, "name":"country 1","code":9}

{"s_type":2, "name":"event-1","code":9}

{"s_type":3, "name":"some other username","code":9}
     */

    static cocos2d::Node *getPlayerInfoView(std::string message);


    static cocos2d::Node *getCountryStatisticsView(std::string message, cocos2d::Size size);

    static cocos2d::Node *getEventStatisticsView(std::string message);

    static cocos2d::Node *getFriendsView(std::string message);

    static cocos2d::Node *getGlobalStatisticsView(string message, cocos2d::Size size);
};


#endif //ARCHERSUNLIMITED_VIEWS_H
