//
// Created by igor on 24.11.16.
//

#ifndef ARCHERSUNLIMITED_VIEWS_H
#define ARCHERSUNLIMITED_VIEWS_H

#include "cocos2d.h"
#include "GameEngine/Global/Misc/SocketClient.h"

class Views : public cocos2d::Node {
public:

    /*
     * {"s_type":1, "name":"global","code":9}

{"s_type":2, "name":"country 1","code":9}

{"s_type":2, "name":"event-1","code":9}

{"s_type":3, "name":"some other username","code":9}
     */

    static cocos2d::Node *getPlayerStatisticsView(std::string message);

    static cocos2d::Node *getPlayerInfoView(std::string message);

    static cocos2d::Node *getInviteView(std::string message);

    static cocos2d::Node *getGlobalStatisticsView(std::string message);

    static cocos2d::Node *getCountryStatisticsView(std::string message);

    static cocos2d::Node *getEventStatisticsView(std::string message);


};


#endif //ARCHERSUNLIMITED_VIEWS_H
