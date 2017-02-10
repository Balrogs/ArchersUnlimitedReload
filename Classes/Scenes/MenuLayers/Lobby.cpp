
#include <Scenes/Layers/BackgroundLayer.h>
#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/Views.h>
#include <GameEngine/Global/Misc/JSONParser.h>
#include <Scenes/PlayLayers/BattleParent.h>
#include <Scenes/PlayLayers/Duel/DuelSceneMultiplayer.h>
#include "Lobby.h"

USING_NS_CC;

bool Lobby::init() {
    if(!Layer::init()){
        return false;
    }

    this->removeAllChildren();
    this->getEventDispatcher()->removeEventListenersForTarget(this);

    _client = SocketClient::getInstance();
    auto visibleSize = Director::getInstance()->getVisibleSize();

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
                                 visibleSize.height - backButton->getBoundingBox().size.height / 2 - 15.f));
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

    _moreInfoBox = Node::create();
    _moreInfoBox->setPosition(visibleSize.width / 2 + 200.f, visibleSize.height * 5 / 8);

    this->addChild(_moreInfoBox);

    _playerInfoBox = Node::create();
    _playerInfoBox->setPosition(150.f, visibleSize.height - 150.f);
    this->addChild(_playerInfoBox);

    _inviteBox = Node::create();
    _inviteBox->setPosition(visibleSize.width - 150.f, visibleSize.height - 30.f);
    this->addChild(_inviteBox);

    _findPlayerButton = cocos2d::ui::Button::create();
    _findPlayerButton->loadTextures(Variables::FIND_BUTTON, Variables::FIND_PRESSED_BUTTON,
                                    Variables::FIND_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);

    _findPlayerButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                //TODO  pass type
                _client->enterLobby(1);
                _findPlayerButton->setVisible(false);
            }
                break;
            default:
                break;
        }
    });
    _inviteBox->addChild(_findPlayerButton);

    _playerGlobalStatisticsBox = Node::create();
    _playerGlobalStatisticsBox->setPosition(visibleSize.width - 200.f, visibleSize.height - 10.f);
    this->addChild(_playerGlobalStatisticsBox);

    _playerCountryStatisticsBox = Node::create();
    _playerCountryStatisticsBox->setPosition(visibleSize.width - 200.f, visibleSize.height - 10.f);
    this->addChild(_playerCountryStatisticsBox);

    _errorMessage = cocos2d::Label::createWithTTF("", Variables::FONT_NAME, 20.f);
    _errorMessage->setPosition(cocos2d::Vec2(_inviteBox->getPosition().x, _inviteBox->getPosition().y - 35.f));
    _errorMessage->setTextColor(Color4B::RED);
    this->addChild(_errorMessage);

    _player2 = nullptr;

    _acceptButton = cocos2d::ui::Button::create();
    _acceptButton->loadTextures(Variables::BUTTON_PATH, Variables::PRESSED_BUTTON_PATH,
                                Variables::BUTTON_PATH, cocos2d::ui::Widget::TextureResType::PLIST);
    _acceptButton->setPosition(
            cocos2d::Vec2(_errorMessage->getPosition().x - 50.f, _errorMessage->getPosition().y - 35.f));
    _acceptButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                _client->enterRoom();
                auto scene = BattleParent::createScene(4);
                Director::getInstance()->pushScene(scene);

                if (auto gameScene = dynamic_cast<DuelSceneMultiplayer *>(BattleParent::getInstance())) {
                    auto player1 = Player::create(_client->getDBPlayer()->getId(), 100,
                                                  _client->getDBPlayer()->getName());
                    gameScene->createPlayers(player1, this->_player2);
                }
            }
                break;
            default:
                break;
        }
    });
    this->addChild(_acceptButton);

    _denyButton = cocos2d::ui::Button::create();
    _denyButton->loadTextures(Variables::CANCEL_BUTTON, Variables::CANCEL_PRESSED_BUTTON, Variables::CANCEL_BUTTON,
                              ui::Widget::TextureResType::PLIST);
    _denyButton->setPosition(cocos2d::Vec2(_acceptButton->getPosition().x + 200.f, _acceptButton->getPosition().y));
    _denyButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                _client->denyInvite();

                deleteInvite();

                _moreInfoBox->removeAllChildren();

                _acceptButton->setEnabled(false);
                _acceptButton->setVisible(false);


                _denyButton->setEnabled(false);
                _denyButton->setVisible(false);
            }
                break;
            default:
                break;

        }
    });
    this->addChild(_denyButton);

    _acceptButton->setEnabled(false);
    _acceptButton->setVisible(false);


    _denyButton->setEnabled(false);
    _denyButton->setVisible(false);

    _client->getPlayerInfo(3, _client->getDBPlayer()->getName());
    _client->getPlayerInfo(1, _client->getDBPlayer()->getName());
    _client->getPlayerInfo(2, _client->getDBPlayer()->getName());

    return true;
}

void Lobby::receiveInvite(string message) {
    _inviteBox->removeAllChildren();
    _inviteBox->addChild(Views::getInviteView(message));
    auto name = JSONParser::parseAnswer(message, "player_name");
    auto id = JSONParser::parseIntAnswer(message, "player_id");
    _player2 = Player::create(id, 100, name);
    _client->getPlayerInfo(3, name);

}

void Lobby::deleteInvite() {
    _inviteBox->removeAllChildren();
    _player2 = nullptr;

    _findPlayerButton = cocos2d::ui::Button::create();
    _findPlayerButton->loadTextures(Variables::FIND_BUTTON, Variables::FIND_PRESSED_BUTTON,
                                    Variables::FIND_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);

    _findPlayerButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                //TODO  pass type
                _client->enterLobby(1);
                _findPlayerButton->setVisible(false);
            }
                break;
            default:
                break;
        }
    });
    _inviteBox->addChild(_findPlayerButton);
}

void Lobby::receivePlayerInfo(string message) {
    auto name = JSONParser::parseAnswer(message, "name");
    if (name == _client->getDBPlayer()->getName()) {
        _playerInfoBox->removeAllChildren();
        _playerInfoBox->addChild(Views::getPlayerInfoView(message));
    } else {
        _moreInfoBox->removeAllChildren();
        auto info = Views::getPlayerInfoView(message);
        info->setVisible(false);

        _moreInfoBox->addChild(info, 1, "info");
        auto moreInfoButton = ui::Button::create();
        moreInfoButton->loadTextures(Variables::MORE_INFO_BUTTON, Variables::MORE_INFO_PRESSED_BUTTON,
                                     Variables::MORE_INFO_BUTTON,
                                     ui::Widget::TextureResType::PLIST);
        moreInfoButton->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type) {
            switch (type) {
                case ui::Widget::TouchEventType::ENDED: {
                    _moreInfoBox->getChildByName("info")->setVisible(true);
                    _moreInfoBox->getChildByName("button")->setVisible(false);
                }
                    break;
                default:
                    break;
            }
        });
        _moreInfoBox->addChild(moreInfoButton, 1, "button");

        _acceptButton->setEnabled(true);
        _acceptButton->setVisible(true);

        _denyButton->setEnabled(true);
        _denyButton->setVisible(true);
    }
}

void Lobby::receiveGlobalStats(string message) {
    _playerGlobalStatisticsBox->removeAllChildren();
    _playerGlobalStatisticsBox->addChild(Views::getPlayerStatisticsView(message));
}

void Lobby::receiveCountryStats(string message) {
    _playerCountryStatisticsBox->removeAllChildren();
    _playerCountryStatisticsBox->addChild(Views::getPlayerStatisticsView(message));
}

void Lobby::joinLobby() {
    _inviteBox->removeAllChildren();

    auto message = cocos2d::Label::createWithTTF("Waiting for opponent...", Variables::FONT_NAME, 25.f);
    _inviteBox->addChild(message);

    _player2 = nullptr;
}