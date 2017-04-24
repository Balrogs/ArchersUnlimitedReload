//
// Created by igor on 24.11.16.
//

#include <GameEngine/Global/Variables.h>
#include "Views.h"
#include "GameEngine/Global/Misc/JSONParser.h"

USING_NS_CC;
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
// TODO  playerview
Node *Views::getPlayerInfoView(std::string message) {
    auto view = Node::create();

    if (message.empty()) {
        return nullptr;
    }

    auto name = JSONParser::parseAnswer(message, "name");
    auto date = JSONParser::parseAnswer(message, "date_reg");
    auto country = JSONParser::parseIntAnswer(message, "country");
    auto b_count = JSONParser::parseIntAnswer(message, "battles_count");
    auto b_win = JSONParser::parseIntAnswer(message, "battles_win");
    auto b_loose = JSONParser::parseIntAnswer(message, "battles_loose");
    auto g_rank = JSONParser::parseIntAnswer(message, "global_rank");
    auto rank = JSONParser::parseIntAnswer(message, "rank");
    auto c_rank = JSONParser::parseIntAnswer(message, "country_rank");


    //   auto player_view = cocos2d::Label::createWithTTF("Name : " + name, "arial.ttf", 25.f);
    auto name_view = cocos2d::Label::createWithTTF("Name : " + name, Variables::FONT_NAME, 25.f);
    auto date_view = cocos2d::Label::createWithTTF("Registration date : " + date, Variables::FONT_NAME, 25.f);
    auto country_view = cocos2d::Label::createWithTTF("Country : " + cocos2d::StringUtils::toString(country),Variables::FONT_NAME, 25.f);

    auto rank_view = cocos2d::Label::createWithTTF("Rank : " + cocos2d::StringUtils::toString(rank), Variables::FONT_NAME, 25.f);
    auto global_rank = cocos2d::Label::createWithTTF("Global : " + cocos2d::StringUtils::toString(g_rank), Variables::FONT_NAME, 25.f);
    auto country_rank = cocos2d::Label::createWithTTF("Country rank : " + cocos2d::StringUtils::toString(c_rank), Variables::FONT_NAME, 25.f);


    auto battles_loose = cocos2d::Label::createWithTTF("Loose : " + cocos2d::StringUtils::toString(b_loose), Variables::FONT_NAME, 25.f);
    auto battles_win = cocos2d::Label::createWithTTF("Win : " + cocos2d::StringUtils::toString(b_win), Variables::FONT_NAME, 25.f);
    auto battles_count = cocos2d::Label::createWithTTF("Total : " + cocos2d::StringUtils::toString(b_count), Variables::FONT_NAME, 25.f);

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

    view->setAnchorPoint(Vec2(x, -y));

    return view;
}

Node *Views::getGlobalStatisticsView(std::string message) {
  //  CCLOG(message.c_str());
    return getStatisticsView(message);
}

Node *Views::getCountryStatisticsView(std::string message) {
 //   CCLOG(message.c_str());
    return getStatisticsView(message);
}

Node *Views::getEventStatisticsView(std::string message) {
  //  CCLOG(message.c_str());
    return getStatisticsView(message);
}

cocos2d::Node *Views::getStatisticsView(std::string message) {
    auto view = Node::create();
    return view;
}

cocos2d::Node *Views::getFriendsView(std::string message) {
    auto view = Node::create();
    return view;
}




