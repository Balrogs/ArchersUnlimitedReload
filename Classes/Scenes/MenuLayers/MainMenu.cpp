//
// Created by igor on 04.10.16.
//

#include <GameEngine/Global/Misc/Views.h>
#include <GameEngine/Global/Misc/JSONParser.h>
#include <Scenes/PlayLayers/DuelSceneMultiplayer.h>
#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/PopUp.h>
#include "MainMenu.h"
#include "Settings.h"

USING_NS_CC;

Scene *MainMenu::createScene() {
    auto scene = Scene::create();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui-0.plist");
    MainMenu *layer = MainMenu::create();
    BackgroundLayer *bg = BackgroundLayer::create();

    scene->addChild(bg, 2);
    scene->addChild(layer, 3);

    //TODO add equipment layer with controls

    return scene;
}


void MainMenu::onEnter() {
    Layer::onEnter();
    SocketClient::destroyInstance();
    this->setVisible(true);
}

void MainMenu::onPushScene(int id) {
    auto scene = BattleScene::createScene(id);
    Director::getInstance()->pushScene(scene);
}

void MainMenu::onChangeLayer() {
    auto scene = MultiplayerMainMenu::createScene();
    Director::getInstance()->pushScene(scene);
}

bool MainMenu::init() {
    if (!Layer::init()) {
        return false;
    }

    // auto item1 = MenuItemFont::create("Waves", CC_CALLBACK_0(MainMenu::onPushScene, this, 0));
    auto item2 = MenuItemFont::create("Apple", CC_CALLBACK_0(MainMenu::onPushScene, this, 1));
    auto item3 = MenuItemFont::create("DUEL", CC_CALLBACK_0(MainMenu::onPushScene, this, 2));
    auto item4 = MenuItemFont::create("DUEL 2 PLAYERS", CC_CALLBACK_0(MainMenu::onPushScene, this, 3));
    auto item5 = MenuItemFont::create("MULTIPLAYER", CC_CALLBACK_0(MainMenu::onChangeLayer, this));

    auto menu = Menu::create(item2, item3, item4, item5, nullptr);
    menu->alignItemsVertically();
    menu->setColor(Color3B::BLACK);
    this->addChild(menu);

    auto settingsButton = cocos2d::ui::Button::create();
    settingsButton->loadTextures(Variables::SETTINGS_BUTTON, Variables::SETTINGS_PRESSED_BUTTON,
                                 Variables::SETTINGS_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);

    auto visibleSize = Director::getInstance()->getVisibleSize();

    settingsButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                this->getParent()->addChild(Settings::create(), 3);
                this->removeFromParent();
            }
                break;
            default:
                break;
        }
    });
    settingsButton->setPosition(Vec2(visibleSize.width - 50.f, 50.f));
    this->addChild(settingsButton);


    const auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
        switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_BREAK:
            case EventKeyboard::KeyCode::KEY_ESCAPE:
            case EventKeyboard::KeyCode::KEY_BACKSPACE: {
                auto popUp = this->getChildByName("PopUp");
                if (popUp == nullptr) {

                    this->getEventDispatcher()->pauseEventListenersForTarget(this, true);

                    auto size = Director::getInstance()->getVisibleSize();

                    auto label = cocos2d::Label::createWithTTF("EXIT THE GAME?", Variables::FONT_NAME,
                                                               Variables::FONT_SIZE);
                    label->setColor(cocos2d::Color3B::BLACK);
                    popUp = MainMenuPopUp::create("ARE YOU SURE?",
                                                  label,
                                                  true);

                    popUp->setPosition(size.width / 2, size.height / 2);
                    this->addChild(popUp, 0, "PopUp");
                } else {
                    popUp->removeFromParent();
                }
            }
                break;
            default:
                break;
        }
    };


    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    //TODO add remove ads button

    //TODO add coins view

    //TODO add open case button

    return true;
}

MultiplayerMainMenu *MultiplayerMainMenu::_instance = nullptr;

cocos2d::Scene *MultiplayerMainMenu::createScene() {
    auto scene = Scene::create();
    MultiplayerMainMenu *layer = MultiplayerMainMenu::getInstance();
    BackgroundLayer *bg = BackgroundLayer::create();

    scene->addChild(bg, 2);
    scene->addChild(layer, 3);

    //TODO add equipment layer without controls
    return scene;
}

MultiplayerMainMenu *MultiplayerMainMenu::getInstance() {
    if (_instance == nullptr) {
        _instance = new MultiplayerMainMenu();
    }
    return _instance;
}

MultiplayerMainMenu::MultiplayerMainMenu() {
    this->schedule(SEL_SCHEDULE(&MultiplayerMainMenu::update), 1.f);
}

void MultiplayerMainMenu::onEnter() {
    Node::onEnter();

    this->removeAllChildren();
    this->getEventDispatcher()->removeEventListenersForTarget(this);

    _client = SocketClient::getInstance();

    auto item1 = MenuItemFont::create("AutoLogin", CC_CALLBACK_0(MultiplayerMainMenu::onPushScene, this, 1));
    auto item2 = MenuItemFont::create("Login", CC_CALLBACK_0(MultiplayerMainMenu::onPushScene, this, 3));
    auto item4 = MenuItemFont::create("Register", CC_CALLBACK_0(MultiplayerMainMenu::onPushScene, this, 2));


    auto editBoxSize = Size(300.f, 50.f);
    auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _editName = ui::EditBox::create(editBoxSize, Variables::BAR, ui::Widget::TextureResType::PLIST);
    _editName->setPosition(
            Vec2(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 5 / 8));
    _editName->setFontName(Variables::FONT_NAME.c_str());
    _editName->setFontColor(Color3B::BLACK);
    _editName->setFontSize(25);
    _editName->setMaxLength(12);
    _editName->setPlaceHolder("NAME:");
    _editName->setPlaceholderFontColor(Color3B::BLACK);
    _editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editName->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);

    _editPassword = ui::EditBox::create(editBoxSize, Variables::BAR, ui::Widget::TextureResType::PLIST);
    _editPassword->setPosition(Vec2(_editName->getPosition().x, _editName->getPosition().y - 60.f));
    _editPassword->setFontName(Variables::FONT_NAME.c_str());
    _editPassword->setFontColor(Color3B::BLACK);
    _editPassword->setFontSize(25);
    _editPassword->setMaxLength(12);
    _editPassword->setPlaceHolder("PASSWORD:");
    _editPassword->setPlaceholderFontColor(Color3B::BLACK);
    _editPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);

    _errorMessage = cocos2d::Label::createWithTTF("", Variables::FONT_NAME, 20.f);
    _errorMessage->setPosition(cocos2d::Vec2(_editPassword->getPosition().x, _editPassword->getPosition().y - 35.f));
    _errorMessage->setTextColor(Color4B::RED);
    this->addChild(_errorMessage);

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
    backButton->setPosition(Vec2(50.f, visibleSize.height - 50.f));
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

    if (_client->connected()) {

        this->addChild(_editName);
        this->addChild(_editPassword);

        auto menu = Menu::create(item1, item2, item4, nullptr);
        menu->alignItemsVertically();
        menu->setColor(Color3B::BLACK);
        menu->setPosition(visibleSize.width / 2, _errorMessage->getPosition().y - 150);
        this->addChild(menu);

    } else {
        auto label = cocos2d::Label::createWithTTF("CONNECTION ERROR", Variables::FONT_NAME,
                                                   Variables::FONT_SIZE);
        label->setColor(cocos2d::Color3B::BLACK);
        auto popUp = MainMenuPopUp::create("", label, false);
        auto visibleSize = Director::getInstance()->getVisibleSize();
        popUp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        this->addChild(popUp, 0, "PopUp");
    }
}

void MultiplayerMainMenu::onPushScene(int id) {
    switch (id) {
        case 1: {
            _client->login();
        }
            break;
        case 2: {
            this->getParent()->addChild(new RegisterMenu());
            this->removeFromParent();
        }
            break;
        case 3: {
            auto name = string(_editName->getText());
            auto password = string(_editPassword->getText());
            if (!name.empty() && !password.empty()) {
                _client->getDBPlayer()->setName(name);
                _client->getDBPlayer()->setPassword(password);
                _client->login();
            } else {
                _errorMessage->setString("Please input your name and password.");
            }
        }
            break;
        default:
            break;
    }
}

void MultiplayerMainMenu::onQuit() {
    Director::getInstance()->popScene();
}


void MultiplayerMainMenu::onError(string message) {
    _errorMessage->setString(message);
}

void MultiplayerMainMenu::update(float dt) {
    if (!_client->connected()) {
        auto label = cocos2d::Label::createWithTTF("CONNECTION ERROR", Variables::FONT_NAME,
                                                   Variables::FONT_SIZE);
        label->setColor(cocos2d::Color3B::BLACK);
        auto popUp = MainMenuPopUp::create("",
                                           label);
        auto visibleSize = Director::getInstance()->getVisibleSize();
        popUp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        this->addChild(popUp, 0, "PopUp");
    }
}

RegisterMenu::RegisterMenu() {

    auto editBoxSize = Size(300.f, 50.f);

    auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _editName = ui::EditBox::create(editBoxSize, Variables::BAR, ui::Widget::TextureResType::PLIST);
    _editName->setPosition(Vec2(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 5 / 8));
    _editName->setFontName(Variables::FONT_NAME.c_str());
    _editName->setFontColor(Color3B::BLACK);
    _editName->setFontSize(25);
    _editName->setMaxLength(12);
    _editName->setPlaceHolder("NAME:");
    _editName->setPlaceholderFontColor(Color3B::BLACK);
    _editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editName->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    this->addChild(_editName);

    _editPassword = ui::EditBox::create(editBoxSize, Variables::BAR, ui::Widget::TextureResType::PLIST);
    _editPassword->setPosition(Vec2(_editName->getPosition().x, _editName->getPosition().y - 60.f));
    _editPassword->setFontName(Variables::FONT_NAME.c_str());
    _editPassword->setFontColor(Color3B::BLACK);
    _editPassword->setFontSize(25);
    _editPassword->setMaxLength(12);
    _editPassword->setPlaceHolder("PASSWORD:");
    _editPassword->setPlaceholderFontColor(Color3B::BLACK);
    _editPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    this->addChild(_editPassword);

    _errorMessage = cocos2d::Label::createWithTTF("", Variables::FONT_NAME, 20.f);
    _errorMessage->setPosition(cocos2d::Vec2(_editPassword->getPosition().x, _editPassword->getPosition().y - 35.f));
    _errorMessage->setTextColor(Color4B::RED);
    this->addChild(_errorMessage);

    auto acceptButton = cocos2d::ui::Button::create();
    acceptButton->loadTextures(Variables::BUTTON_PATH, Variables::PRESSED_BUTTON_PATH,
                               Variables::BUTTON_PATH, cocos2d::ui::Widget::TextureResType::PLIST);

    acceptButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                auto name = string(_editName->getText());
                auto password = string(_editPassword->getText());
                if (!name.empty() && !password.empty()) {
                    _client->registerUser(name, 1, password);
                } else {
                    _errorMessage->setString("Please input your name and password.");
                }
            }
                break;
            default:
                break;
        }
    });
    acceptButton->setPosition(Vec2(visibleSize.width / 2, _errorMessage->getPosition().y - 100));
    this->addChild(acceptButton);

    _client = SocketClient::getInstance();

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
    backButton->setPosition(Vec2(50.f, visibleSize.height - 50.f));
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
}

void RegisterMenu::onQuit() {
    this->getParent()->addChild(MultiplayerMainMenu::getInstance());
    this->removeFromParent();
}


LobbyLayer *LobbyLayer::_instance = nullptr;

LobbyLayer *LobbyLayer::getInstance() {
    if (_instance == nullptr) {
        _instance = new LobbyLayer();
    }
    return _instance;
}

cocos2d::Scene *LobbyLayer::createScene() {
    auto scene = Scene::create();
    LobbyLayer *layer = LobbyLayer::getInstance();
    BackgroundLayer *bg = BackgroundLayer::create();

    scene->addChild(bg, 2);
    scene->addChild(layer, 3);
    return scene;
}

LobbyLayer::LobbyLayer() {

}

void LobbyLayer::onEnter() {
    Node::onEnter();

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
    backButton->setPosition(Vec2(50.f, visibleSize.height - 50.f));
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
                _client->enterLobby();
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
                auto scene = BattleScene::createScene(4);
                Director::getInstance()->pushScene(scene);

                if (auto gameScene = dynamic_cast<DuelSceneMultiplayer *>(BattleScene::instance)) {
                    auto player1 = Player::create(_client->getDBPlayer()->getId(), 100,
                                                  _client->getDBPlayer()->getName());
                    gameScene->createPlayers(player1, LobbyLayer::getInstance()->_player2);
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
}

void LobbyLayer::receiveInvite(string message) {
    _inviteBox->removeAllChildren();
    _inviteBox->addChild(Views::getInviteView(message));
    auto name = JSONParser::parseAnswer(message, "player_name");
    auto id = JSONParser::parseIntAnswer(message, "player_id");
    _player2 = Player::create(id, 100, name);
    _client->getPlayerInfo(3, name);

}

void LobbyLayer::deleteInvite() {
    _inviteBox->removeAllChildren();
    _player2 = nullptr;
}


void LobbyLayer::receivePlayerInfo(string message) {
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

void LobbyLayer::receiveGlobalStats(string message) {
    _playerGlobalStatisticsBox->removeAllChildren();
    _playerGlobalStatisticsBox->addChild(Views::getPlayerStatisticsView(message));
}

void LobbyLayer::receiveCountryStats(string message) {
    _playerCountryStatisticsBox->removeAllChildren();
    _playerCountryStatisticsBox->addChild(Views::getPlayerStatisticsView(message));
}

void LobbyLayer::joinLobby() {
    _inviteBox->removeAllChildren();

    auto message = cocos2d::Label::createWithTTF("Waiting for opponent...", Variables::FONT_NAME, 25.f);
    _inviteBox->addChild(message);

    _player2 = nullptr;
}
