#include <GameEngine/Global/Variables.h>
#include <Localization/LocalizedStrings.h>
#include <ui/UIButton.h>
#include <Scenes/MenuLayers/Multiplayer/Lobby.h>
#include <Scenes/MenuLayers/Main/MainMenu.h>
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
// TODO customize
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

Node *Views::getStatisticsView(std::string message, cocos2d::Size size) {
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

Node *Views::getEventStatisticsView(std::string message) {
    auto view = Node::create();
    return view;
}

std::vector<cocos2d::Node*> Views::getFriendsView(std::string message, cocos2d::Size size) {
    auto friends = JSONParser::parseFriends(message);
    std::vector<cocos2d::Node*>  views;
    if(friends.size() == 0){
        auto label = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("NO FRIENDS"),
                                                   Variables::FONT_NAME,
                                                   Variables::FONT_SIZE(),
                                                   size,
                                                   TextHAlignment::LEFT,
                                                   TextVAlignment::CENTER);
        label->setColor(Color3B::GRAY);
        views.push_back(label);
        return views;
    }
    for(auto friendItem : friends){
        views.push_back(friendItem->getView(size));
    }
    return views;
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


const cocos2d::Color3B FriendView::PLAYER_ONLINE_COLOR = Color3B::BLACK;
const cocos2d::Color3B FriendView::PLAYER_OFFLINE_COLOR = Color3B::GRAY;

FriendView::FriendView(string name, int id, bool isOnline) {
    _name = name;
    _isOnline = isOnline;
    _id = id;
}

string FriendView::getPlayerName() {
    return _name;
}

cocos2d::Node *FriendView::getView(cocos2d::Size size) {
    Node* view = Node::create();
    view->setContentSize(size);

    Color3B color;
    if(_isOnline){
        color = PLAYER_ONLINE_COLOR;
    } else {
        color = PLAYER_OFFLINE_COLOR;
    }

    auto nameSize = _getNameSize(size);
    auto dotSize = _getDotSize(size);

    auto button = cocos2d::ui::Button::create();
    button->addTouchEventListener(
            [&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
                switch (type) {
                    case cocos2d::ui::Widget::TouchEventType::ENDED: {
                        if (auto lobby = dynamic_cast<Lobby *>(MainScene::getInstance()->getMain())) {
                            if(auto child = lobby->getChildByName("PopUp")) {
                                child->removeFromParent();
                            }
                            auto popUp = GameTypePopUp::create(_name, _id);
                            auto visibleSize = Director::getInstance()->getVisibleSize();
                            popUp->setPosition(visibleSize.width / 2, - visibleSize.height);
                            lobby->addChild(popUp, 10, "PopUp");
                        }
                        break;
                    }
                    default:
                        break;
                }
            });
    button->setTitleText(_name);
    button->setTitleFontSize(Variables::FONT_SIZE());
    button->setTitleFontName(Variables::FONT_NAME);
    button->setColor(color);
    //auto labelRank = cocos2d::Label::createWithTTF(StringUtils::toString(""), Variables::FONT_NAME, Variables::FONT_SIZE(), nameSize, TextHAlignment::RIGHT, TextVAlignment::CENTER);

    button->setPosition(Vec2(button->getContentSize().width / 2, size.height / 2));
   // labelRank->setPosition(Vec2(size.width - rankSize.width / 2, size.height / 2));

    view->addChild(button);
   // view->addChild(labelRank);

    return view;
}

cocos2d::Size FriendView::_getNameSize(cocos2d::Size size) {
    return Size(
            0.8f * size.width,
            size.height
    );
}

cocos2d::Size FriendView::_getDotSize(cocos2d::Size size) {
    return Size(
            0.2f * size.width,
            size.height
    );
}
