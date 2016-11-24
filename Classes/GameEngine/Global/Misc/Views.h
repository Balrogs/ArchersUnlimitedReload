//
// Created by igor on 24.11.16.
//

#ifndef ARCHERSUNLIMITED_VIEWS_H
#define ARCHERSUNLIMITED_VIEWS_H

#include "cocos2d.h"
#include "SocketClient.h"

class Views : public cocos2d::Node {
public:

    /*
     * {"s_type":1, "name":"global","code":9}

{"s_type":2, "name":"country 1","code":9}

{"s_type":2, "name":"event-1","code":9}

{"s_type":3, "name":"some other username","code":9}
     */

    static cocos2d::Node *getPlayerStatisticsView(SocketClient *sc);

    static cocos2d::Node *getPlayerInfoView(std::string name, SocketClient *sc);

    static cocos2d::Node *getPlayerInfoView(SocketClient *sc);

    static cocos2d::Node *getInviteView(std::string message);

private:

    static cocos2d::Node *getGlobalStatisticsView(std::string name, SocketClient *sc);

    static cocos2d::Node *getCountryStatisticsView(std::string name, SocketClient *sc);

    static cocos2d::Node *getEventStatisticsView(std::string name, SocketClient *sc);


};


#endif //ARCHERSUNLIMITED_VIEWS_H