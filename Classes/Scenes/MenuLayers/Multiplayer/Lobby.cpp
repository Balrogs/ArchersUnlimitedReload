
#include <Scenes/Layers/BackgroundLayer.h>
#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/UI/Views.h>
#include <GameEngine/Global/Misc/JSONParser.h>
#include <Scenes/PlayLayers/BattleParent.h>
#include <Scenes/PlayLayers/Duel/DuelSceneMultiplayer.h>
#include <GameEngine/Global/Misc/UI/PopUp.h>
#include <Localization/LocalizedStrings.h>
#include "Lobby.h"
#include "Scenes/MenuLayers/Main/MainMenu.h"
#include "EventView.h"

USING_NS_CC;

bool Lobby::init() {
    if(!Layer::init()){
        return false;
    }

    _client = SocketClient::getInstance();
    _visibleSize = Director::getInstance()->getVisibleSize();

   _leftPart1 = cocos2d::Node::create();
   _leftPart2 = cocos2d::Node::create();
   _rightPart1 = cocos2d::Node::create();
   _rightPart2 = cocos2d::Node::create();

    _leftPart1->setPositionY(_visibleSize.height);
    _leftPart2->setPositionX(-_visibleSize.width);
    _rightPart1->setPositionY(-_visibleSize.height);
    _rightPart2->setPositionX(_visibleSize.width);

    this->addChild(_leftPart1);
    this->addChild(_leftPart2);
    this->addChild(_rightPart1);
    this->addChild(_rightPart2);

    auto backButton = cocos2d::ui::Button::create();
    backButton->loadTextures(Variables::BACK_BUTTON_PATH, Variables::BACK_PRESSED_BUTTON_PATH,
                             Variables::BACK_BUTTON_PATH, cocos2d::ui::Widget::TextureResType::PLIST);

    backButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                onQuit();
            }
                break;
            default:
                break;
        }
    });
    backButton->setPosition(Vec2(backButton->getBoundingBox().size.width / 2 + 15.f,
                                 _visibleSize.height - backButton->getBoundingBox().size.height / 2 - 15.f));
    _leftPart1->addChild(backButton);

    const auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
        switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_BREAK:
            case EventKeyboard::KeyCode::KEY_ESCAPE:
            case EventKeyboard::KeyCode::KEY_BACKSPACE: {
                auto popUp = this->getChildByName("PopUp");
                if (popUp == nullptr) {
                    onQuit();
                } else {
                    _setSearchButtonState();
                    popUp->removeFromParent();
                }
            }
                break;
            default:
                break;
        }
    };

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    auto infoBoxSize = Size(_visibleSize.width / 2, 3 * _visibleSize.height / 5);

    _playerInfo = Node::create();
    _leftPart2->addChild(_playerInfo, 3);
    _playerGlobalStatistics = Node::create();
    _leftPart2->addChild(_playerGlobalStatistics, 2);
    _playerCountryStatistics = Node::create();
    _leftPart2->addChild(_playerCountryStatistics, 2);

    _playerInfoButton = cocos2d::ui::Button::create();
    _playerInfoButton->loadTextures(Variables::BUTTON_1, Variables::BUTTON_1, Variables::BUTTON_1,
                                    cocos2d::ui::Widget::TextureResType::PLIST);

    _playerInfoButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                _reloadInfoBox(1);
            }
                break;
            default:
                break;
        }
    });

    auto infoButtonScale = infoBoxSize.width / 3 / _playerInfoButton->getContentSize().width;

    _playerInfoButton->setScale(infoButtonScale);

    _playerInfoButton->setPosition(
            Vec2(_playerInfoButton->getBoundingBox().size.width / 2 + 15.f,
                 backButton->getPosition().y - backButton->getBoundingBox().size.height / 2 -
                 _playerInfoButton->getBoundingBox().size.height / 2 - 15.f));

    auto _playerInfoButton_label = cocos2d::Label::createWithTTF(
            LocalizedStrings::getInstance()->getString("INFO"), Variables::FONT_NAME,
            Variables::FONT_SIZE());
    _playerInfoButton_label->setPosition(_playerInfoButton->getPosition());
    _leftPart2->addChild(_playerInfoButton_label, 4);

    _playerInfo->addChild(_playerInfoButton, 2);

    _playerInfoBox = Sprite::createWithSpriteFrameName(Variables::BACK_1);

    _playerInfoBox->setScale(infoBoxSize.width / _playerInfoBox->getContentSize().width,
                             infoBoxSize.height / _playerInfoBox->getContentSize().height);

    _playerInfoBox->setPosition(infoBoxSize.width / 2 + 15.f,
                                _playerInfoButton->getBoundingBox().getMinY() - _playerInfoBox->getBoundingBox().size.height / 2 + 7.f);

    _playerInfo->addChild(_playerInfoBox, 1);

    _friendsBox = Sprite::createWithSpriteFrameName(Variables::BG1);

    _friendsBox->setScale((_visibleSize.width / 3) / _friendsBox->getContentSize().width,
                          (3 * _visibleSize.height / 5) / _friendsBox->getContentSize().height);

    _friendsBox->setPosition(10 * _visibleSize.width / 11 - _friendsBox->getBoundingBox().size.height / 2 ,
                             3 * _visibleSize.height / 5
    );

    auto title = cocos2d::Label::createWithTTF( LocalizedStrings::getInstance()->getString("FRIENDS"), Variables::FONT_NAME,
                                                Variables::FONT_SIZE());
    title->setPosition(_friendsBox->getPosition().x,
                       _friendsBox->getBoundingBox().getMaxY() - title->getContentSize().height / 2);
    _rightPart2->addChild(title, 2);

    _rightPart2->addChild(_friendsBox, 1);

    _findFriendButton = cocos2d::ui::Button::create();
    _findFriendButton->loadTextures(Variables::FIND_BUTTON, Variables::FIND_PRESSED_BUTTON,
                                    Variables::FIND_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);

    _findFriendButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                _showPopUp(InputNamePopUp::create());
            }
                break;
            default:
                break;
        }
    });
    _findFriendButton->setPosition(Vec2(_friendsBox->getPosition().x,
                                        _friendsBox->getBoundingBox().getMinY() + _findFriendButton->getContentSize().height / 2 + Variables::FONT_SIZE() / 2));
    _rightPart2->addChild(_findFriendButton, 2);

    _findPlayerButton = RichSearchButton::create();
    _findPlayerButton->setPosition(Vec2(
            _friendsBox->getPosition().x - _findFriendButton->getBoundingBox().size.width,
            _playerInfoBox->getBoundingBox().getMinY() + _findPlayerButton->getBoundingBox().size.height / 2
    ));
    _rightPart1->addChild(_findPlayerButton);

    auto eventButton = cocos2d::ui::Button::create();
    eventButton->loadTextures(Variables::EVENT_BUTTON, Variables::EVENT_PRESSED_BUTTON,
                              Variables::EVENT_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);

    eventButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                this->runAction(Sequence::create(
                        CallFunc::create([&](){
                            MainScene::getInstance()->wait(false);

                            _leftPart1->runAction(MoveTo::create(0.5f, Vec2(0, _visibleSize.height)));
                            _leftPart2->runAction(MoveTo::create(0.5f, Vec2(-_visibleSize.width, 0)));
                            _rightPart1->runAction(MoveTo::create(0.5f, Vec2(0, -_visibleSize.height)));
                            _rightPart2->runAction(MoveTo::create(0.5f, Vec2(_visibleSize.width, 0)));
                        }),
                        CallFunc::create([](){
                            MainScene::getInstance()->pushMain(EventView::create());
                        }), NULL)
                );
            }
                break;
            default:
                break;
        }
    });
    eventButton->setPosition(Vec2(
            _findPlayerButton->getPosition().x + 2.f * eventButton->getBoundingBox().size.width,
            _findPlayerButton->getPosition().y
    ));
    _rightPart1->addChild(eventButton);

    _showScrollView();


    _playerGlobalStatisticsBox = Sprite::createWithSpriteFrameName(Variables::BACK_2);

    _playerGlobalStatisticsBox->setScale(infoBoxSize.width / _playerInfoBox->getContentSize().width,
                                         infoBoxSize.height / _playerInfoBox->getContentSize().height);

    _playerGlobalStatisticsBox->setPosition(_playerInfoBox->getPosition());

    _playerGlobalStatistics->addChild(_playerGlobalStatisticsBox, 1);

    _playerGlobalStatisticsButton = cocos2d::ui::Button::create();
    _playerGlobalStatisticsButton->loadTextures(Variables::BUTTON_2, Variables::BUTTON_2, Variables::BUTTON_2,
                                                cocos2d::ui::Widget::TextureResType::PLIST);

    _playerGlobalStatisticsButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                _reloadInfoBox(2);
            }
                break;
            default:
                break;
        }
    });
    _playerGlobalStatisticsButton->setScale(infoButtonScale);

    _playerGlobalStatisticsButton->setPosition(
            Vec2(_playerInfoButton->getPosition().x + _playerGlobalStatisticsButton->getBoundingBox().size. width,
                 _playerInfoButton->getPosition().y));
    auto _playerGlobalStatisticsButton_label = cocos2d::Label::createWithTTF(
            LocalizedStrings::getInstance()->getString("GLOBAL"), Variables::FONT_NAME,
            Variables::FONT_SIZE());
    _playerGlobalStatisticsButton_label->setPosition(_playerGlobalStatisticsButton->getPosition());
    _leftPart2->addChild(_playerGlobalStatisticsButton_label, 4);

    _playerGlobalStatistics->addChild(_playerGlobalStatisticsButton, 1);

    _playerCountryStatisticsBox = Sprite::createWithSpriteFrameName(Variables::BACK_3);

    _playerCountryStatisticsBox->setScale(infoBoxSize.width / _playerInfoBox->getContentSize().width,
                                          infoBoxSize.height / _playerInfoBox->getContentSize().height);

    _playerCountryStatisticsBox->setPosition(_playerGlobalStatisticsBox->getPosition());

    _playerCountryStatistics->addChild(_playerCountryStatisticsBox, 1);

    _playerCountryStatisticsButton = cocos2d::ui::Button::create();
    _playerCountryStatisticsButton->loadTextures(Variables::BUTTON_3, Variables::BUTTON_3, Variables::BUTTON_3,
                                                 cocos2d::ui::Widget::TextureResType::PLIST);

    _playerCountryStatisticsButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                _reloadInfoBox(3);
            }
                break;
            default:
                break;
        }
    });

    _playerCountryStatisticsButton->setScale(infoButtonScale);

    _playerCountryStatisticsButton->setPosition(
            Vec2(_playerGlobalStatisticsButton->getPosition().x + _playerCountryStatisticsButton->getBoundingBox().size.width,
                 _playerInfoButton->getPosition().y));
    auto _playerCountryStatisticsButton_label = cocos2d::Label::createWithTTF(
            LocalizedStrings::getInstance()->getString("REGION"), Variables::FONT_NAME,
            Variables::FONT_SIZE());
    _playerCountryStatisticsButton_label->setPosition(_playerCountryStatisticsButton->getPosition());
    _leftPart2->addChild(_playerCountryStatisticsButton_label, 4);

    _playerCountryStatistics->addChild(_playerCountryStatisticsButton, 2);

    _reloadInfoBox(1);

    _errorMessage = cocos2d::Label::createWithTTF("", Variables::FONT_NAME, 20.f);
    _errorMessage->setPosition(cocos2d::Vec2(_findPlayerButton->getPosition().x, _findPlayerButton->getPosition().y + _visibleSize.height / 20));
    _errorMessage->setTextColor(Color4B::RED);
    this->addChild(_errorMessage);

    return true;
}

void Lobby::receiveInvite(string message) {
    _player2Name = JSONParser::parseAnswer(message, "player_name");
    _player2Id = JSONParser::parseIntAnswer(message, "player_id");
    //   _gameType = JSONParser::parseIntAnswer(message, "game_type");

    _client->getPlayerInfo(3, _player2Name);
}

void Lobby::deleteInvite() {
    _setSearchButtonState();

    _resetPlayer();

}

void Lobby::receivePlayerInfo(string message) {
    auto name = JSONParser::parseAnswer(message, "name");
    if (name == _client->getDBPlayer()->getName()) {
        _client->getDBPlayer()->setCountry(JSONParser::parseIntAnswer(message, "country"));
        auto view = Views::getPlayerInfoView(message);
        view->setPosition(Vec2(_playerInfoBox->getPosition().x,
                               0.9f * _playerInfoBox->getBoundingBox().getMaxY()));
        _playerInfo->addChild(view, 2);

        _scrollView->removeAllChildren();
        auto size = Size(_scrollView->getInnerContainerSize().width, Variables::FONT_SIZE());
        auto friendsView = Views::getFriendsView(message, size);
        for (unsigned long i = 0; i < friendsView.size(); i++) {
            auto friendItem = friendsView.at(i);
            friendItem->setPosition(Vec2(0, _scrollView->getInnerContainerSize().height - 25.f - (i > 0) * 25.f - (i + 1) * size.height / 2));
            _scrollView->addChild(friendItem, 3);
        }
        this->addChild(_scrollView, 4);

    } else {
        _showPopUp(InvitePopUp::create(LocalizedStrings::getInstance()->getString("INVITE"),
                                      Views::getPlayerInfoView(message), true));
        _setSearchButtonState();
    }
}

void Lobby::receiveGlobalStats(string message) {
    auto size = _playerGlobalStatisticsBox->getBoundingBox().size * 0.95f;
    auto marginSize = _playerGlobalStatisticsBox->getBoundingBox().size * 0.05f / 2.f;
    auto view = Views::getStatisticsView(message, size);
    if(auto child = _playerGlobalStatistics->getChildByName("table")){
        child->removeFromParent();
    }
    view->setPosition(Vec2(
            _playerGlobalStatisticsBox->getBoundingBox().getMinX() + marginSize.width,
            _playerGlobalStatisticsBox->getBoundingBox().getMaxY() - marginSize.height

    ));
    _playerGlobalStatistics->addChild(view, 2, "table");
}

void Lobby::receiveCountryStats(string message) {
    auto size = _playerCountryStatisticsBox->getBoundingBox().size * 0.95f;
    auto marginSize = _playerCountryStatisticsBox->getBoundingBox().size * 0.05f / 2.f;
    auto view = Views::getStatisticsView(message, size);
    if(auto child = _playerCountryStatistics->getChildByName("table")){
        child->removeFromParent();
    }
    view->setPosition(Vec2(
            _playerCountryStatisticsBox->getBoundingBox().getMinX() + marginSize.width,
            _playerCountryStatisticsBox->getBoundingBox().getMaxY() - marginSize.height

    ));
    _playerCountryStatistics->addChild(view, 2, "table");
}

void Lobby::joinLobby() {
    _resetPlayer();
}

void Lobby::acceptInvite() {
    _client->enterRoom();
    auto scene = BattleParent::createScene(_gameType);
    Director::getInstance()->pushScene(scene);

    if (auto gameScene = dynamic_cast<MultiplayerBattle *>(BattleParent::getInstance())) {
        gameScene->createPlayers(_player2Id, _player2Name);
    }
}

void Lobby::denyInvite() {
    _client->denyInvite();
    deleteInvite();
}

void Lobby::_setSearchButtonState() {
    _findPlayerButton->reset();
}

void Lobby::showSearchPopUp(){
    _showPopUp(GameTypePopUp::create());
}

void Lobby::_showPopUp(PopUp *popUp) {
    if(auto child = this->getChildByName("PopUp")) {
        child->removeFromParent();
    }
    popUp->setPosition(_visibleSize.width / 2, - _visibleSize.height);
    this->addChild(popUp, 10, "PopUp");
}

void Lobby::startSearch( int gameType) {
    _gameType = gameType;
    _client->enterLobby(_gameType);
}

void Lobby::addFriend(string name) {
    _client->addToFriends(name);
}

void Lobby::_showScrollView() {

    _scrollView = cocos2d::ui::ScrollView::create();
    _scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    _scrollView->setContentSize(Size(0.9f * _friendsBox->getBoundingBox().size.width,
                                     0.9f * (_friendsBox->getBoundingBox().size.height - _findFriendButton->getBoundingBox().size.height - 1.5f * Variables::FONT_SIZE())));
    _scrollView->setInnerContainerSize(
            Size(0.9f * _friendsBox->getContentSize().width,
                 2 * _friendsBox->getContentSize().height));
    _scrollView->setBackGroundImage(Variables::GRAY_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);
    _scrollView->setBackGroundImageScale9Enabled(true);
    _scrollView->setBounceEnabled(true);
    _scrollView->setInertiaScrollEnabled(true);
    _scrollView->setPosition(Vec2(_friendsBox->getPosition().x - _scrollView->getContentSize().width / 2,
                                  _friendsBox->getBoundingBox().getMinY() + _findFriendButton->getContentSize().height + Variables::FONT_SIZE()));
    _rightPart2->addChild(_scrollView, 4);

}

void Lobby::_reloadInfoBox(int type) {
    switch(type){
        case 1: {

            _playerInfo->setLocalZOrder(3);
            _playerGlobalStatistics->setLocalZOrder(2);
            _playerCountryStatistics->setLocalZOrder(2);

            break;
        }
        case 2: {
            _playerInfo->setLocalZOrder(2);
            _playerGlobalStatistics->setLocalZOrder(3);
            _playerCountryStatistics->setLocalZOrder(2);

            break;
        }
        case 3: {
            _playerInfo->setLocalZOrder(2);
            _playerGlobalStatistics->setLocalZOrder(2);
            _playerCountryStatistics->setLocalZOrder(3);

            break;
        }
        default: {
            break;
        }
    }
}

void Lobby::onEnter() {
    Node::onEnter();

    _reloadInfoBox(1);

    _setSearchButtonState();

    _resetPlayer();

    MainScene::getInstance()->wait(false);

    _leftPart1->runAction(MoveTo::create(0.4f, Vec2::ZERO));
    _leftPart2->runAction(MoveTo::create(0.4f, Vec2::ZERO));
    _rightPart1->runAction(MoveTo::create(0.4f, Vec2::ZERO));
    _rightPart2->runAction(MoveTo::create(0.4f, Vec2::ZERO));

    _client->getPlayerInfo(1, "global");
    _client->getPlayerInfo(2, StringUtils::format("country-%d", _client->getDBPlayer()->getCountry()));
    _client->getPlayerInfo(3, _client->getDBPlayer()->getName());
}

void Lobby::leaveLobby() {
   _client->leaveLobby();
}

void Lobby::_resetPlayer() {
    _player2Id = 0;
    _player2Name = "";

}

void Lobby::onError(string message) {
    _errorMessage->setString(message);
}

void Lobby::onQuit() {
    this->getEventDispatcher()->pauseEventListenersForTarget(this, true);
    this->runAction(Sequence::create(
            CallFunc::create([&](){
                MainScene::getInstance()->wait(false);

                _leftPart1->runAction(MoveTo::create(0.5f, Vec2(0, _visibleSize.height)));
                _leftPart2->runAction(MoveTo::create(0.5f, Vec2(-_visibleSize.width, 0)));
                _rightPart1->runAction(MoveTo::create(0.5f, Vec2(0, -_visibleSize.height)));
                _rightPart2->runAction(MoveTo::create(0.5f, Vec2(_visibleSize.width, 0)));
            }),
            DelayTime::create(0.5f),
            CallFunc::create([](){
                MainScene::getInstance()->popAndReplace();
            }), NULL)
    );
}

void Lobby::inviteFriend(string name, int id, int gameType) {
    _client->invite(name, id, gameType);
}
