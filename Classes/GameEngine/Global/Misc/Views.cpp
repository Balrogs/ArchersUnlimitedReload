//
// Created by igor on 24.11.16.
//

#include "Views.h"
#include "JSONParser.h"

USING_NS_CC;

// TODO  playerview
Node *Views::getPlayerInfoView(std::string name, SocketClient *sc) {
    auto view = Node::create();

    auto message = sc->getPlayerInfo(3, name);
    if (message.empty()) {
        return nullptr;
    }

    auto date = JSONParser::parseAnswer(message, "date_reg");
    auto country = JSONParser::parseIntAnswer(message, "country");
    auto b_count = JSONParser::parseIntAnswer(message, "battles_count");
    auto b_win = JSONParser::parseIntAnswer(message, "battles_win");
    auto b_loose = JSONParser::parseIntAnswer(message, "battles_loose");
    auto g_rank = JSONParser::parseIntAnswer(message, "global_rank");
    auto rank = JSONParser::parseIntAnswer(message, "rank");
    auto c_rank = JSONParser::parseIntAnswer(message, "country_rank");


    //   auto player_view = cocos2d::Label::createWithTTF("Name : " + name, "arial.ttf", 25.f);
    auto name_view = cocos2d::Label::createWithTTF("Name : " + name, "arial.ttf", 25.f);
    auto date_view = cocos2d::Label::createWithTTF("Registration date : " + date, "arial.ttf", 25.f);
    auto country_view = cocos2d::Label::createWithTTF("Country : " + to_string(country), "arial.ttf", 25.f);

    auto rank_view = cocos2d::Label::createWithTTF("Rank : " + to_string(rank), "arial.ttf", 25.f);
    auto global_rank = cocos2d::Label::createWithTTF("Global : " + to_string(g_rank), "arial.ttf", 25.f);
    auto country_rank = cocos2d::Label::createWithTTF("Country rank : " + to_string(c_rank), "arial.ttf", 25.f);


    auto battles_loose = cocos2d::Label::createWithTTF("Loose : " + to_string(b_loose), "arial.ttf", 25.f);
    auto battles_win = cocos2d::Label::createWithTTF("Win : " + to_string(b_win), "arial.ttf", 25.f);
    auto battles_count = cocos2d::Label::createWithTTF("Total : " + to_string(b_count), "arial.ttf", 25.f);

    int x = 0;
    int y = 0;

    name_view->setPosition(x, y);
    date_view->setPosition(x, y-=30);
    country_view->setPosition(x, y-=30);

    rank_view->setPosition(x, y-=30);
    global_rank->setPosition(x, y-=30);
    country_rank->setPosition(x, y-=30);

    battles_loose->setPosition(x, y-=30);
    battles_win->setPosition(x, y-=30);
    battles_count->setPosition(x, y-=30);

    view->addChild(name_view);
    view->addChild(date_view);
    view->addChild(country_view);

    view->addChild(rank_view);
    view->addChild(global_rank);
    view->addChild(country_rank);

    view->addChild(battles_win);
    view->addChild(battles_loose);
    view->addChild(battles_count);

    return view;
}

/*
 * {"name":"some username",
 * "rank":1,
 * "battles_count":0,
 * "country":1,
 * "date_reg":"2016-11-21T15:36:26.182",
 * "battles_loose":0,
 * "code":9,
 * "playerView":{"color":{"red":0,"green":0,"blue":0},"helmet":0,"hood":0,"bow":0},
 * "country_rank":1,
 * "global_rank":1,
 * "battles_win":0}

 */


// TODO ADD FRIENDS VIEW
Node *Views::getPlayerInfoView(SocketClient *sc) {
    auto view = Node::create();
    auto name = sc->getName();

    auto message = sc->getPlayerInfo(3, name);
    if (message.empty()) {
        return nullptr;
    }

    auto date = JSONParser::parseAnswer(message, "date_reg");
    auto country = JSONParser::parseIntAnswer(message, "country");
    auto b_count = JSONParser::parseIntAnswer(message, "battles_count");
    auto b_win = JSONParser::parseIntAnswer(message, "battles_win");
    auto b_loose = JSONParser::parseIntAnswer(message, "battles_loose");
    auto g_rank = JSONParser::parseIntAnswer(message, "global_rank");
    auto rank = JSONParser::parseIntAnswer(message, "rank");
    auto c_rank = JSONParser::parseIntAnswer(message, "country_rank");


 //   auto player_view = cocos2d::Label::createWithTTF("Name : " + name, "arial.ttf", 25.f);
    auto name_view = cocos2d::Label::createWithTTF("Name : " + name, "arial.ttf", 25.f);
    auto date_view = cocos2d::Label::createWithTTF("Registration date : " + date, "arial.ttf", 25.f);
    auto country_view = cocos2d::Label::createWithTTF("Country : " + to_string(country), "arial.ttf", 25.f);

    auto rank_view = cocos2d::Label::createWithTTF("Rank : " + to_string(rank), "arial.ttf", 25.f);
    auto global_rank = cocos2d::Label::createWithTTF("Global : " + to_string(g_rank), "arial.ttf", 25.f);
    auto country_rank = cocos2d::Label::createWithTTF("Country rank : " + to_string(c_rank), "arial.ttf", 25.f);


    auto battles_loose = cocos2d::Label::createWithTTF("Loose : " + to_string(b_loose), "arial.ttf", 25.f);
    auto battles_win = cocos2d::Label::createWithTTF("Win : " + to_string(b_win), "arial.ttf", 25.f);
    auto battles_count = cocos2d::Label::createWithTTF("Total : " + to_string(b_count), "arial.ttf", 25.f);

    int x = 0;
    int y = 0;

    name_view->setPosition(x, y);
    date_view->setPosition(x, y-=30);
    country_view->setPosition(x, y-=30);

    rank_view->setPosition(x, y-=30);
    global_rank->setPosition(x, y-=30);
    country_rank->setPosition(x, y-=30);

    battles_loose->setPosition(x, y-=30);
    battles_win->setPosition(x, y-=30);
    battles_count->setPosition(x, y-=30);

    view->addChild(name_view);
    view->addChild(date_view);
    view->addChild(country_view);

    view->addChild(rank_view);
    view->addChild(global_rank);
    view->addChild(country_rank);

    view->addChild(battles_win);
    view->addChild(battles_loose);
    view->addChild(battles_count);

    return view;
}

Node *Views::getInviteView(std::string message) {
    auto view = Node::create();

    auto name = JSONParser::parseAnswer(message, "player_name");
    auto rank = JSONParser::parseIntAnswer(message, "player_rank");

    auto name_view = cocos2d::Label::createWithTTF("Name : " + name, "arial.ttf", 25.f);
    auto rank_view = cocos2d::Label::createWithTTF("Rank : " + to_string(rank), "arial.ttf", 25.f);
    rank_view->setPosition(name_view->getPosition().x, name_view->getPosition().y - 30.f);
    view->addChild(name_view);
    view->addChild(rank_view);

    return view;
}

Node *Views::getPlayerStatisticsView(SocketClient *sc) {
    auto view = Node::create();
    return view;
}


Node *Views::getGlobalStatisticsView(std::string name, SocketClient *sc) {
    auto view = Node::create();
    return view;
}

Node *Views::getCountryStatisticsView(std::string name, SocketClient *sc) {
    auto view = Node::create();
    return view;
}

Node *Views::getEventStatisticsView(std::string name, SocketClient *sc) {
    auto view = Node::create();
    return view;
}




