#include <Scenes/Layers/BackgroundLayer.h>
#include <GameEngine/Global/Variables.h>
#include <Localization/LocalizedStrings.h>
#include <GameEngine/Global/Misc/PopUp.h>
#include "MultiplayerMenu.h"
#include "RegisterMenu.h"

USING_NS_CC;

MultiplayerMenu *MultiplayerMenu::_instance = nullptr;

cocos2d::Scene *MultiplayerMenu::createScene() {
    auto scene = Scene::create();
    MultiplayerMenu *layer = new MultiplayerMenu();
    BackgroundLayer *bg = BackgroundLayer::create();

    scene->addChild(bg, 2);
    scene->addChild(layer, 3);

    //TODO add equipment layer without controls
    return scene;
}

MultiplayerMenu *MultiplayerMenu::getInstance() {
    if (_instance == nullptr) {
        _instance = new MultiplayerMenu();
    }
    return _instance;
}

MultiplayerMenu::MultiplayerMenu() {
    this->schedule(SEL_SCHEDULE(&MultiplayerMenu::update), 1.f);
}

void MultiplayerMenu::onEnter() {
    Node::onEnter();

    this->removeAllChildren();
    this->getEventDispatcher()->removeEventListenersForTarget(this);

    _client = SocketClient::getInstance();

    auto item1 = MenuItemFont::create("AutoLogin", CC_CALLBACK_0(MultiplayerMenu::onPushScene, this, 1));
    auto item2 = MenuItemFont::create("Login", CC_CALLBACK_0(MultiplayerMenu::onPushScene, this, 3));
    auto item4 = MenuItemFont::create("Register", CC_CALLBACK_0(MultiplayerMenu::onPushScene, this, 2));


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

    if (_client->connected()) {

        this->addChild(_editName);
        this->addChild(_editPassword);

        auto menu = Menu::create(item1, item2, item4, nullptr);
        menu->alignItemsVertically();
        menu->setColor(Color3B::BLACK);
        menu->setPosition(visibleSize.width / 2, _errorMessage->getPosition().y - 150);
        this->addChild(menu);

    } else {
        auto label = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("CONNECTION ERROR"),
                                                   Variables::FONT_NAME, Variables::FONT_SIZE);
        label->setColor(cocos2d::Color3B::BLACK);
        auto popUp = MainMenuPopUp::create("", label, false);
        popUp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        this->addChild(popUp, 0, "PopUp");
    }
}

void MultiplayerMenu::onPushScene(int id) {
    switch (id) {
        case 1: {
            _client->login();
        }
            break;
        case 2: {
            this->getParent()->addChild(RegisterMenu::create(), 3);
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

void MultiplayerMenu::onQuit() {
    Director::getInstance()->popScene();
}

void MultiplayerMenu::onError(string message) {
    _errorMessage->setString(message);
}

void MultiplayerMenu::update(float dt) {
    if (!_client->connected() && this->getChildByName("PopUp") == nullptr) {
        auto label = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("CONNECTION ERROR"),
                                                   Variables::FONT_NAME,
                                                   Variables::FONT_SIZE);
        label->setColor(cocos2d::Color3B::BLACK);
        auto popUp = MainMenuPopUp::create("",
                                           label);
        auto visibleSize = Director::getInstance()->getVisibleSize();
        popUp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        this->addChild(popUp, 0, "PopUp");
    }
}