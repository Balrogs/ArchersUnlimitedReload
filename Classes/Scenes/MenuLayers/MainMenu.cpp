//
// Created by igor on 04.10.16.
//

#include <GameEngine/Global/Misc/Views.h>
#include <GameEngine/Global/Misc/JSONParser.h>
#include "MainMenu.h"
#include "Scenes/PlayLayers/Battle.h"

USING_NS_CC;

Scene *MainMenu::createScene() {
    auto scene = Scene::create();
    MainMenu *layer = MainMenu::create();
    scene->addChild(layer);
    return scene;
}

MainMenu::MainMenu() {
    // auto item1 = MenuItemFont::create("Waves", CC_CALLBACK_0(MainMenu::onPushScene, this, 0));
    auto item2 = MenuItemFont::create("Apple", CC_CALLBACK_0(MainMenu::onPushScene, this, 1));
    auto item3 = MenuItemFont::create("Duel with bot", CC_CALLBACK_0(MainMenu::onPushScene, this, 2));
    auto item4 = MenuItemFont::create("Duel 2P", CC_CALLBACK_0(MainMenu::onPushScene, this, 3));
    auto item5 = MenuItemFont::create("Duel Multiplayer", CC_CALLBACK_0(MainMenu::onChangeLayer, this, 4));
    auto item6 = MenuItemFont::create("Quit", CC_CALLBACK_1(MainMenu::onQuit, this));

    auto menu = Menu::create(item2, item3, item4, item5, item6, nullptr);
    menu->alignItemsVertically();

    this->addChild(menu);
}

MainMenu::~MainMenu() {

}

void MainMenu::onPushScene(int id) {
    auto scene = BattleScene::createScene(id);
    Director::getInstance()->pushScene(scene);
}

void MainMenu::onChangeLayer(int id) {
    this->getParent()->addChild(new MultiplayerMainMenu());
    this->removeFromParent();
}

void MainMenu::onQuit(cocos2d::Ref *sender) {
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

}


MultiplayerMainMenu::MultiplayerMainMenu() {
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
            this->getParent()->addChild(new LoginLayer());
            this->removeFromParent();
        }
            break;
        default:
            break;
    }
}

void MultiplayerMainMenu::onQuit(cocos2d::Ref *sender) {
    MainMenu *layer = MainMenu::create();
    this->getParent()->addChild(layer);
    this->removeFromParent();
}

void MultiplayerMainMenu::onEnter() {
    Node::onEnter();

    this->removeAllChildren();

    _client = SocketClient::getInstance();

    auto item1 = MenuItemFont::create("AutoLogin", CC_CALLBACK_0(MultiplayerMainMenu::onPushScene, this, 1));
    auto item2 = MenuItemFont::create("Login", CC_CALLBACK_0(MultiplayerMainMenu::onPushScene, this, 3));
    auto item4 = MenuItemFont::create("Register", CC_CALLBACK_0(MultiplayerMainMenu::onPushScene, this, 2));
    auto item6 = MenuItemFont::create("Back", CC_CALLBACK_1(MultiplayerMainMenu::onQuit, this));

    if (_client->connected()) {
        auto menu = Menu::create(item1, item2, item4, item6, nullptr);
        menu->alignItemsVertically();
        this->addChild(menu);

    } else {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto _errorMessage = cocos2d::Label::createWithTTF("", "arial.ttf", 25.f);
        _errorMessage->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height * 5 / 8));
        _errorMessage->setTextColor(Color4B::RED);
        _errorMessage->setString("Can't connect to the server...");
        this->addChild(_errorMessage);

        auto menu = Menu::create(item6, nullptr);
        menu->alignItemsVertically();
        menu->setPosition(visibleSize.width / 2, _errorMessage->getPosition().y - 100);
        this->addChild(menu);
    }
}


RegisterMenu::RegisterMenu() {

    auto editBoxSize = Size(300.f, 50.f);

    auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    std::string pNormalSprite = "bar.png";
    _editName = cocos2d::ui::EditBox::create(editBoxSize, ui::Scale9Sprite::create(pNormalSprite));
    _editName->setPosition(Vec2(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 5 / 8));
    _editName->setFontName("Paint Boy");
    _editName->setFontSize(25);
    _editName->setMaxLength(12);
    _editName->setPlaceHolder("Name:");
    _editName->setPlaceholderFontColor(Color3B::WHITE);
    _editName->setMaxLength(8);
    _editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    this->addChild(_editName);

    _editPassword = ui::EditBox::create(editBoxSize, "bar.png");
    _editPassword->setPosition(Vec2(_editName->getPosition().x, _editName->getPosition().y - 60.f));
    _editPassword->setFont("American Typewriter", 25);
    _editPassword->setPlaceHolder("Password:");
    _editPassword->setMaxLength(12);
    _editPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    this->addChild(_editPassword);

    _errorMessage = cocos2d::Label::createWithTTF("", "arial.ttf", 20.f);
    _errorMessage->setPosition(cocos2d::Vec2(_editPassword->getPosition().x, _editPassword->getPosition().y - 35.f));
    _errorMessage->setTextColor(Color4B::RED);
    this->addChild(_errorMessage);

    auto item4 = MenuItemFont::create("Register", CC_CALLBACK_0(RegisterMenu::onPushScene, this, 2));
    auto item6 = MenuItemFont::create("Back", CC_CALLBACK_1(RegisterMenu::onQuit, this));

    auto menu = Menu::create(item4, item6, nullptr);
    menu->alignItemsVertically();
    menu->setPosition(visibleSize.width / 2, _errorMessage->getPosition().y - 100);
    _client = SocketClient::getInstance();
    this->addChild(menu);
}

void RegisterMenu::onPushScene(int id) {
    switch (id) {
        case 2: {
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
}

void RegisterMenu::onQuit(cocos2d::Ref *sender) {
    this->getParent()->addChild(new MultiplayerMainMenu());
    this->removeFromParent();
}

void RegisterMenu::onExit() {
    Node::onExit();
    
    this->getParent()->addChild(new MultiplayerMainMenu());
    this->removeFromParent();
}

LoginLayer::LoginLayer() {
    auto editBoxSize = Size(300.f, 50.f);

    auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    std::string pNormalSprite = "bar.png";
    _editName = cocos2d::ui::EditBox::create(editBoxSize, ui::Scale9Sprite::create(pNormalSprite));
    _editName->setPosition(Vec2(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 5 / 8));
    _editName->setFontName("Paint Boy");
    _editName->setFontSize(25);
    _editName->setMaxLength(12);
    _editName->setPlaceHolder("Name:");
    _editName->setPlaceholderFontColor(Color3B::WHITE);
    _editName->setMaxLength(8);
    _editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    this->addChild(_editName);

    _editPassword = ui::EditBox::create(editBoxSize, "bar.png");
    _editPassword->setPosition(Vec2(_editName->getPosition().x, _editName->getPosition().y - 60.f));
    _editPassword->setFont("American Typewriter", 25);
    _editPassword->setPlaceHolder("Password:");
    _editPassword->setMaxLength(12);
    _editPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    this->addChild(_editPassword);

    _errorMessage = cocos2d::Label::createWithTTF("", "arial.ttf", 20.f);
    _errorMessage->setPosition(cocos2d::Vec2(_editPassword->getPosition().x, _editPassword->getPosition().y - 35.f));
    _errorMessage->setTextColor(Color4B::RED);
    this->addChild(_errorMessage);

    auto item4 = MenuItemFont::create("Login", CC_CALLBACK_0(LoginLayer::onPushScene, this, 2));
    auto item6 = MenuItemFont::create("Back", CC_CALLBACK_1(LoginLayer::onQuit, this));

    auto menu = Menu::create(item4, item6, nullptr);
    menu->alignItemsVertically();
    menu->setPosition(visibleSize.width / 2, _errorMessage->getPosition().y - 100);
    _client = SocketClient::getInstance();
    this->addChild(menu);
}

void LoginLayer::onPushScene(int id) {
    switch (id) {
        case 2: {
            auto name = string(_editName->getText());
            auto password = string(_editPassword->getText());
            if (!name.empty() && !password.empty()) {
                _client->getDBPlayer()->setId(atoi(name.c_str()));
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

void LoginLayer::onQuit(cocos2d::Ref *sender) {
    this->getParent()->addChild(new MultiplayerMainMenu());
    this->removeFromParent();
}


static LobbyLayer *_instance = nullptr;

LobbyLayer *LobbyLayer::getInstance() {
    if (_instance == nullptr) {
        _instance = new LobbyLayer();
    }
    return _instance;
}

cocos2d::Scene *LobbyLayer::createScene() {
    auto scene = Scene::create();
    LobbyLayer *layer = LobbyLayer::getInstance();
    scene->addChild(layer);
    return scene;
}


LobbyLayer::LobbyLayer() {

}

void LobbyLayer::onQuit(cocos2d::Ref *sender) {
    Director::getInstance()->popScene();
}


void LobbyLayer::onEnter() {
    Node::onEnter();
    this->removeAllChildren();

    _client = SocketClient::getInstance();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto message = cocos2d::Label::createWithTTF("Waiting for opponent...", "arial.ttf", 25.f);

    _moreInfoBox = Node::create();
    _moreInfoBox->setPosition(visibleSize.width / 2 + 200.f, visibleSize.height * 5 / 8);

    this->addChild(_moreInfoBox);

    _playerInfoBox = Node::create();
    _playerInfoBox->setPosition(300.f, visibleSize.height - 30.f);
    this->addChild(_playerInfoBox);

    _inviteBox = Node::create();
    _inviteBox->setPosition(_playerInfoBox->getPosition().x, visibleSize.height * 5 / 8);
    _inviteBox->addChild(message);
    this->addChild(_inviteBox);

    _playerGlobalStatisticsBox = Node::create();
    _playerGlobalStatisticsBox->setPosition(visibleSize.width - 200.f, visibleSize.height - 10.f);
    this->addChild(_playerGlobalStatisticsBox);

    _playerCountryStatisticsBox = Node::create();
    _playerCountryStatisticsBox->setPosition(visibleSize.width - 200.f, visibleSize.height - 10.f);
    this->addChild(_playerCountryStatisticsBox);

    _errorMessage = cocos2d::Label::createWithTTF("", "arial.ttf", 20.f);
    _errorMessage->setPosition(cocos2d::Vec2(_inviteBox->getPosition().x, _inviteBox->getPosition().y - 35.f));
    _errorMessage->setTextColor(Color4B::RED);
    this->addChild(_errorMessage);

    _acceptButton = cocos2d::ui::Button::create("bar.png");
    _acceptButton->setScale(0.5f);
    _acceptButton->setTitleFontSize(32);
    _acceptButton->setTitleColor(Color3B::WHITE);
    _acceptButton->setTitleText("ACCEPT");

    _acceptButton->setPosition(cocos2d::Vec2(_errorMessage->getPosition().x, _errorMessage->getPosition().y - 35.f));
    _acceptButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                _client->enterRoom();
                auto scene = BattleScene::createScene(4);
                Director::getInstance()->pushScene(scene);

            }
                break;
            default:
                break;
        }
    });
    this->addChild(_acceptButton);

    auto quit = MenuItemFont::create("Back", CC_CALLBACK_1(LobbyLayer::onQuit, this));

    auto menu = Menu::create(quit, nullptr);
    menu->alignItemsVertically();
    menu->setPosition(visibleSize.width / 2, _acceptButton->getPosition().y - 100);
    this->addChild(menu);


    _acceptButton->setEnabled(false);
    _acceptButton->setVisible(false);
    _client->getPlayerInfo(3, _client->getDBPlayer()->getName());
    _client->getPlayerInfo(1, _client->getDBPlayer()->getName());
    _client->getPlayerInfo(2, _client->getDBPlayer()->getName());
}

void LobbyLayer::receiveInvite(string message) {
    _inviteBox->removeAllChildren();
    _inviteBox->addChild(Views::getInviteView(message));
    auto name = JSONParser::parseAnswer(message, "player_name");
    _client->getPlayerInfo(3, name);
    _acceptButton->setEnabled(true);
    _acceptButton->setVisible(true);
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
        auto moreInfoButton = ui::Button::create("bar.png");
        moreInfoButton->setTitleText("more info");
        moreInfoButton->setTitleFontSize(25);
        moreInfoButton->setTitleColor(Color3B::WHITE);
        moreInfoButton->setScale(0.5f);
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
