//
// Created by igor on 24.11.16.
//

#include <GameEngine/Global/Variables.h>
#include <Localization/LocalizedStrings.h>
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

Node *Views::getGlobalStatisticsView(std::string message, cocos2d::Size size) {
    auto length = JSONParser::getListSize(message);
    auto view = RankTable::create(length, size);
    auto rankings = JSONParser::parseRankings(message);
    auto name = SocketClient::getInstance()->getDBPlayer()->getName();
    view->addRank(RankView::getHeader(view->getRankSize()), 0);
    // test
    // view->addRank((new RankView("123456789012", 3, 12345678))->getView(view->getRankSize(), false), 4);
    for(int i = 0; i < rankings.size(); i++){
        view->addRank(rankings[i]->getView(view->getRankSize(), name == rankings[i]->getPlayerName()), i + 1);
    }
    return view;
}

Node *Views::getCountryStatisticsView(std::string message, cocos2d::Size size) {
    auto length = JSONParser::getListSize(message);
    auto view = RankTable::create(length, size);
    auto rankings = JSONParser::parseRankings(message);
    auto name = SocketClient::getInstance()->getDBPlayer()->getName();
    view->addRank(RankView::getHeader(view->getRankSize()), 0);
    for(int i = 0; i < rankings.size(); i++){
        view->addRank(rankings[i]->getView(view->getRankSize(), name == rankings[i]->getPlayerName()), i + 1);
    }
    return view;
}

Node *Views::getEventStatisticsView(std::string message) {
    auto view = Node::create();
    return view;
}

cocos2d::Node *Views::getFriendsView(std::string message) {
    auto view = Node::create();
    return view;
}

RankViewBase *RankViewBase::create(cocos2d::Size size, cocos2d::Color4B color) {
    RankViewBase *ret = new(std::nothrow) RankViewBase();
    if (ret && ret->init(size, color)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool RankViewBase::init(cocos2d::Size size, cocos2d::Color4B color) {
    if(!Node::init()){
        return  false;
    }

    _bg = cocos2d::LayerColor::create(color, size.width,  size.height);
    this->addChild(_bg);

    return true;
}

const cocos2d::Color4B RankView::BG_COLOR = Color4B(0, 0, 0, 0);
const cocos2d::Color4B RankView::PLAYER_COLOR = Color4B(0, 0, 0, 160);
const cocos2d::Color4B RankView::HEADER_COLOR = Color4B(0, 0, 0, 50);

RankView::RankView(string name, int country, int rank) {
    _name = name;
    _country = country;
    _rank = rank;
}

string RankView::getPlayerName() {
    return _name;
}

int RankView::getCountry() {
    return _country;
}

int RankView::getRank() {
    return _rank;
}

cocos2d::Node *RankView::getHeader(cocos2d::Size size) {
    auto view = RankViewBase::create(size, HEADER_COLOR);

    auto nameSize = _getNameSize(size);
    auto rankSize = _getRankSize(size);

    auto labelName = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("NAME"), Variables::FONT_NAME, Variables::FONT_SIZE(), nameSize, TextHAlignment::CENTER, TextVAlignment::CENTER);
    auto labelRank = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("RANK"), Variables::FONT_NAME, Variables::FONT_SIZE(), rankSize, TextHAlignment::CENTER, TextVAlignment::CENTER);

    labelName->setPosition(Vec2(nameSize.width / 2, size.height / 2));
    labelRank->setPosition(Vec2(size.width - rankSize.width / 2, size.height / 2));

    view->addChild(labelName);
    view->addChild(labelRank);

    return view;
}

cocos2d::Node *RankView::getView(cocos2d::Size size, bool isPlayer) {
    RankViewBase* view;

    if(isPlayer){
        view  = RankViewBase::create(size, PLAYER_COLOR);
    } else {
        view  = RankViewBase::create(size, BG_COLOR);
    }

    auto nameSize = _getNameSize(size);
    auto rankSize = _getRankSize(size);
    auto countrySize = _getRegionSize(size);

    auto labelName = cocos2d::Label::createWithTTF(_name, Variables::FONT_NAME, Variables::FONT_SIZE(),nameSize, TextHAlignment::LEFT, TextVAlignment::CENTER);
    auto labelRank = cocos2d::Label::createWithTTF(StringUtils::toString(_rank), Variables::FONT_NAME, Variables::FONT_SIZE(), rankSize, TextHAlignment::RIGHT, TextVAlignment::CENTER);
    auto labelCountry = cocos2d::Label::createWithTTF(StringUtils::toString(_country), Variables::FONT_NAME, Variables::FONT_SIZE(), countrySize, TextHAlignment::LEFT, TextVAlignment::CENTER);

    labelName->setPosition(Vec2(nameSize.width / 2, size.height / 2));
    labelRank->setPosition(Vec2(size.width - rankSize.width / 2, size.height / 2));
    labelCountry->setPosition(Vec2(size.width - rankSize.width - countrySize.width / 2, size.height / 2));

    view->addChild(labelName);
    view->addChild(labelRank);
    view->addChild(labelCountry);
    return view;
}

cocos2d::Size RankView::_getNameSize(cocos2d::Size size) {
    return Size(
            0.5f * size.width,
            size.height
    );
}

cocos2d::Size RankView::_getRegionSize(cocos2d::Size size) {
    return Size(
            size.width / 5,
            size.height
    );
}

cocos2d::Size RankView::_getRankSize(cocos2d::Size size) {
    return Size(
            3 * size.width / 10,
            size.height
    );
}


RankTable *RankTable::create(int length, cocos2d::Size size) {
    RankTable *ret = new(std::nothrow) RankTable();
    if (ret && ret->init(length, size)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool RankTable::init(int length, cocos2d::Size size) {
    if(!Node::init()){
        return  false;
    }

    _size = length;
    _rankSize = Size(size.width, size.height / 11);

    return true;
}

void RankTable::addRank(cocos2d::Node* rankView, int id) {
    rankView->setPosition(Vec2(
            0,
            - _rankSize.height * (id + 1)
    ));
    this->addChild(rankView);
}

cocos2d::Size RankTable::getRankSize() {
    return _rankSize;
}
