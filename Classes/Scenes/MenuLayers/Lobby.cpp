
#include <Scenes/Layers/BackgroundLayer.h>
#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/Views.h>
#include <GameEngine/Global/Misc/JSONParser.h>
#include <Scenes/PlayLayers/BattleParent.h>
#include <Scenes/PlayLayers/Duel/DuelSceneMultiplayer.h>
#include <GameEngine/Global/Misc/PopUp.h>
#include <Localization/LocalizedStrings.h>
#include "Lobby.h"

USING_NS_CC;

bool Lobby::init() {
    if(!Layer::init()){
        return false;
    }

    this->removeAllChildren();
    this->getEventDispatcher()->removeEventListenersForTarget(this);

    _client = SocketClient::getInstance();
    _visibleSize = Director::getInstance()->getVisibleSize();

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
    this->addChild(backButton);

    const auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
        switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_BREAK:
            case EventKeyboard::KeyCode::KEY_ESCAPE:
            case EventKeyboard::KeyCode::KEY_BACKSPACE: {
                onQuit();
            }
                break;
            default:
                break;
        }
    };

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    auto infoBoxSize = Size(_visibleSize.width / 2, 3 * _visibleSize.height / 5);

    _playerInfo = Node::create();
    this->addChild(_playerInfo, 3);
    _playerGlobalStatistics = Node::create();
    this->addChild(_playerGlobalStatistics, 2);
    _playerCountryStatistics = Node::create();
    this->addChild(_playerCountryStatistics, 2);

    _playerInfoButton = cocos2d::ui::Button::create();
    _playerInfoButton->loadTextures(Variables::RED_BUTTON, Variables::RED_PRESSED_BUTTON, Variables::RED_BUTTON,
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
            Variables::FONT_SIZE);
    _playerInfoButton_label->setPosition(_playerInfoButton->getContentSize().width / 2,
                                         _playerInfoButton->getContentSize().height / 2);
    _playerInfoButton->addChild(_playerInfoButton_label, 4);

    _playerInfo->addChild(_playerInfoButton, 2);

    _playerInfoBox = Sprite::createWithSpriteFrameName(Variables::BG1);

    _playerInfoBox->setScale(infoBoxSize.width / _playerInfoBox->getContentSize().width,
                             infoBoxSize.height / _playerInfoBox->getContentSize().height);

    _playerInfoBox->setPosition(infoBoxSize.width / 2 + 15.f,
                                _playerInfoButton->getBoundingBox().getMinY() - _playerInfoBox->getBoundingBox().size.height / 2 + 15.f);

    _playerInfo->addChild(_playerInfoBox, 1);

    _inviteBox = Node::create();
    _inviteBox->setPosition(4 * _visibleSize.width / 5, _visibleSize.height / 10);
    this->addChild(_inviteBox);

    _findPlayerButton = cocos2d::ui::Button::create();
    _findPlayerButton->loadTextures(Variables::FIND_BUTTON, Variables::FIND_PRESSED_BUTTON,
                                    Variables::FIND_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);

    _findPlayerButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                _showPopUp(GameTypePopUp::create());
            }
                break;
            default:
                break;
        }
    });
    _inviteBox->addChild(_findPlayerButton);


    _friendsBox = Sprite::createWithSpriteFrameName(Variables::BG1);

    _friendsBox->setScale((_visibleSize.width / 3) / _friendsBox->getContentSize().width,
                          (3 * _visibleSize.height / 5) / _friendsBox->getContentSize().height);

    _friendsBox->setPosition(10 * _visibleSize.width / 11 - _friendsBox->getBoundingBox().size.height / 2 ,
                             3 * _visibleSize.height / 5
    );

    auto title = cocos2d::Label::createWithTTF( LocalizedStrings::getInstance()->getString("FRIENDS"), Variables::FONT_NAME,
                                                Variables::FONT_SIZE);
    title->setPosition(_friendsBox->getPosition().x,
                       _friendsBox->getBoundingBox().getMaxY() - title->getContentSize().height / 2);
    this->addChild(title, 2);

    this->addChild(_friendsBox, 1);

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
                                        _friendsBox->getBoundingBox().getMinY() + _findFriendButton->getContentSize().height / 2 + Variables::FONT_SIZE / 2));
    this->addChild(_findFriendButton, 2);

    _showScrollView();


    _playerGlobalStatisticsBox = Sprite::createWithSpriteFrameName(Variables::BG1);

    _playerGlobalStatisticsBox->setScale(infoBoxSize.width / _playerInfoBox->getContentSize().width,
                             infoBoxSize.height / _playerInfoBox->getContentSize().height);

    _playerGlobalStatisticsBox->setPosition(infoBoxSize.width / 2 + 15.f,
                                _playerInfoButton->getBoundingBox().getMinY() - _playerGlobalStatisticsBox->getBoundingBox().size.height / 2 + 15.f);

    _playerGlobalStatistics->addChild(_playerGlobalStatisticsBox, 1);
    
    _playerGlobalStatisticsButton = cocos2d::ui::Button::create();
    _playerGlobalStatisticsButton->loadTextures(Variables::RED_BUTTON, Variables::RED_PRESSED_BUTTON, Variables::RED_BUTTON,
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
            Variables::FONT_SIZE);
    _playerGlobalStatisticsButton_label->setPosition(_playerGlobalStatisticsButton->getContentSize().width / 2,
                                                     _playerGlobalStatisticsButton->getContentSize().height / 2);
    _playerGlobalStatisticsButton->addChild(_playerGlobalStatisticsButton_label, 4);

    _playerGlobalStatistics->addChild(_playerGlobalStatisticsButton, 1);

    _playerCountryStatisticsBox = Sprite::createWithSpriteFrameName(Variables::BG1);

    _playerCountryStatisticsBox->setScale(infoBoxSize.width / _playerInfoBox->getContentSize().width,
                                         infoBoxSize.height / _playerInfoBox->getContentSize().height);

    _playerCountryStatisticsBox->setPosition(infoBoxSize.width / 2 + 15.f,
                                            _playerInfoButton->getBoundingBox().getMinY() - _playerCountryStatisticsBox->getBoundingBox().size.height / 2 + 15.f);

    _playerCountryStatistics->addChild(_playerCountryStatisticsBox, 1);

    _playerCountryStatisticsButton = cocos2d::ui::Button::create();
    _playerCountryStatisticsButton->loadTextures(Variables::RED_BUTTON, Variables::RED_PRESSED_BUTTON, Variables::RED_BUTTON,
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
            Variables::FONT_SIZE);
    _playerCountryStatisticsButton_label->setPosition(_playerCountryStatisticsButton->getContentSize().width / 2,
                                                      _playerCountryStatisticsButton->getContentSize().height / 2);
    _playerCountryStatisticsButton->addChild(_playerCountryStatisticsButton_label, 4);

    _playerCountryStatistics->addChild(_playerCountryStatisticsButton, 2);

    _reloadInfoBox(1);

    _errorMessage = cocos2d::Label::createWithTTF("", Variables::FONT_NAME, 20.f);
    _errorMessage->setPosition(cocos2d::Vec2(_inviteBox->getPosition().x, _inviteBox->getPosition().y + _visibleSize.height / 20));
    _errorMessage->setTextColor(Color4B::RED);
    this->addChild(_errorMessage);

    _player2 = nullptr;

    _client->getPlayerInfo(3, _client->getDBPlayer()->getName());
    _client->getPlayerInfo(1, _client->getDBPlayer()->getName());
    _client->getPlayerInfo(2, _client->getDBPlayer()->getName());

    return true;
}

void Lobby::receiveInvite(string message) {
    auto name = JSONParser::parseAnswer(message, "player_name");
    auto id = JSONParser::parseIntAnswer(message, "player_id");
    //   _gameType = JSONParser::parseIntAnswer(message, "game_type");
    _player2 = Player::create(id, 100, name);
    _client->getPlayerInfo(3, name);
}

void Lobby::deleteInvite() {
    _reloadInviteBox(true);

    _player2 = nullptr;
}

void Lobby::receivePlayerInfo(string message) {
    auto name = JSONParser::parseAnswer(message, "name");
    if (name == _client->getDBPlayer()->getName()) {
        auto view = Views::getPlayerInfoView(message);
        view->setPosition(Vec2(_playerInfoBox->getPosition().x,
                               0.9f * _playerInfoBox->getBoundingBox().getMaxY()));
        _playerInfo->addChild(view, 2);
    } else {
        _showPopUp(InvitePopUp::create(LocalizedStrings::getInstance()->getString("INVITE"), Views::getPlayerInfoView(message), true));
        _reloadInviteBox(false);
    }
}

void Lobby::receiveGlobalStats(string message) {
    auto view = Views::getPlayerInfoView(message);
    _playerGlobalStatistics->addChild(view, 2);
}

void Lobby::receiveCountryStats(string message) {
    auto view = Views::getPlayerInfoView(message);
    _playerCountryStatistics->addChild(view, 2);
}

void Lobby::joinLobby() {
    _reloadInviteBox(false);

    _player2 = nullptr;
}

void Lobby::acceptInvite() {
    _client->enterRoom();
    auto scene = BattleParent::createScene(_gameType);
    Director::getInstance()->pushScene(scene);

    if (auto gameScene = dynamic_cast<DuelSceneMultiplayer *>(BattleParent::getInstance())) {
        auto player1 = Player::create(_client->getDBPlayer()->getId(), 100,
                                      _client->getDBPlayer()->getName());
        gameScene->createPlayers(player1, this->_player2);
    }
}

void Lobby::denyInvite() {
    _client->denyInvite();
    deleteInvite();
}

void Lobby::_reloadInviteBox(bool isEmpty) {
    _findPlayerButton->setVisible(isEmpty);
    //TODO init _loading
//    _loading->setVisible(!isEmpty);
}

void Lobby::_showPopUp(PopUp *popUp) {
    if(auto child = this->getChildByName("PopUp")) {
        child->removeFromParent();
    }
    popUp->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);
    this->addChild(popUp, 10, "PopUp");
}

void Lobby::startSearch(int gameType) {
    _gameType = gameType;
    _client->enterLobby(_gameType);
    _reloadInviteBox(false);
}

void Lobby::addFriend(string name) {
    _client->addToFriends(name);
}


//TODO
void Lobby::_showScrollView() {

    _scrollView = cocos2d::ui::ScrollView::create();
    _scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    _scrollView->setContentSize(Size(0.9f * _friendsBox->getBoundingBox().size.width,
                                     0.9f * (_friendsBox->getBoundingBox().size.height - _findFriendButton->getBoundingBox().size.height - 1.5f * Variables::FONT_SIZE)));
    _scrollView->setInnerContainerSize(
            Size(0.9f * _friendsBox->getContentSize().width,
                 2 * _friendsBox->getContentSize().height));
    _scrollView->setBackGroundImage(Variables::GRAY_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);
    _scrollView->setBackGroundImageScale9Enabled(true);
    _scrollView->setBounceEnabled(true);
    _scrollView->setInertiaScrollEnabled(true);
    _scrollView->setPosition(Vec2(_friendsBox->getPosition().x - _scrollView->getContentSize().width / 2,
                                  _friendsBox->getBoundingBox().getMinY() + _findFriendButton->getContentSize().height + Variables::FONT_SIZE));
//    
//        for (unsigned long i = 0; i < language_list.size(); i++) {
//            auto language = language_list.at(i);
//            auto languageButton = cocos2d::ui::Button::create();
//            languageButton->setTitleText(language);
//            languageButton->setTitleFontSize(Variables::FONT_SIZE);
//            languageButton->setTitleFontName(Variables::FONT_NAME);
//            languageButton->setColor(Color3B::BLACK);
//            languageButton->addTouchEventListener(
//                    [&, language](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
//                        switch (type) {
//                            case cocos2d::ui::Widget::TouchEventType::ENDED: {
//                                cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();
//                                def->setStringForKey("LANGUAGE", language);
//                                onQuit();
//                                break;
//                            }
//                            default:
//                                break;
//                        }
//                    });
//            languageButton->setPosition(Vec2(_scrollView->getInnerContainerSize().width / 2,
//                                             _scrollView->getInnerContainerSize().height - 25.f -
//                                             i * _languageBox->getContentSize().height));
//            _scrollView->addChild(languageButton, 3);
//
//        }
    this->addChild(_scrollView, 4);

}

void Lobby::_reloadInfoBox(int type) {
    switch(type){
        case 1: {

            _playerInfo->setLocalZOrder(3);
            _playerGlobalStatistics->setLocalZOrder(2);
            _playerCountryStatistics->setLocalZOrder(2);


            _playerInfoButton->loadTextures(Variables::GREEN_BUTTON, Variables::GREEN_PRESSED_BUTTON, Variables::GREEN_BUTTON,
                                            cocos2d::ui::Widget::TextureResType::PLIST);
            _playerGlobalStatisticsButton->loadTextures(Variables::RED_BUTTON, Variables::RED_PRESSED_BUTTON, Variables::RED_BUTTON,
                                            cocos2d::ui::Widget::TextureResType::PLIST);

            _playerCountryStatisticsButton->loadTextures(Variables::RED_BUTTON, Variables::RED_PRESSED_BUTTON, Variables::RED_BUTTON,
                                            cocos2d::ui::Widget::TextureResType::PLIST);

            break;
        }
        case 2: {
            _playerInfo->setLocalZOrder(2);
            _playerGlobalStatistics->setLocalZOrder(3);
            _playerCountryStatistics->setLocalZOrder(2);


            _playerInfoButton->loadTextures(Variables::RED_BUTTON, Variables::RED_PRESSED_BUTTON, Variables::RED_BUTTON,
                                            cocos2d::ui::Widget::TextureResType::PLIST);
            _playerGlobalStatisticsButton->loadTextures(Variables::GREEN_BUTTON, Variables::GREEN_PRESSED_BUTTON, Variables::GREEN_BUTTON,
                                                        cocos2d::ui::Widget::TextureResType::PLIST);
            _playerCountryStatisticsButton->loadTextures(Variables::RED_BUTTON, Variables::RED_PRESSED_BUTTON, Variables::RED_BUTTON,
                                                         cocos2d::ui::Widget::TextureResType::PLIST);

            break;
        }
        case 3: {
            _playerInfo->setLocalZOrder(2);
            _playerGlobalStatistics->setLocalZOrder(2);
            _playerCountryStatistics->setLocalZOrder(3);

            _playerInfoButton->loadTextures(Variables::RED_BUTTON, Variables::RED_PRESSED_BUTTON, Variables::RED_BUTTON,
                                            cocos2d::ui::Widget::TextureResType::PLIST);
            _playerGlobalStatisticsButton->loadTextures(Variables::RED_BUTTON, Variables::RED_PRESSED_BUTTON, Variables::RED_BUTTON,
                                                        cocos2d::ui::Widget::TextureResType::PLIST);
            _playerCountryStatisticsButton->loadTextures(Variables::GREEN_BUTTON, Variables::GREEN_PRESSED_BUTTON, Variables::GREEN_BUTTON,
                                                         cocos2d::ui::Widget::TextureResType::PLIST);

            break;
        }
        default: {
            break;
        }
    }
}
