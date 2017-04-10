#include <Scenes/Layers/BackgroundLayer.h>
#include <GameEngine/Global/Variables.h>
#include <Localization/LocalizedStrings.h>
#include <GameEngine/Global/Misc/PopUp.h>
#include "MultiplayerMenu.h"
#include "RegisterMenu.h"
#include "Scenes/MenuLayers/Main/MainMenu.h"

USING_NS_CC;

bool MultiplayerMenu::init() {
    if(!Layer::init()){
        return false;
    }

    auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
    _visibleSize = Director::getInstance()->getVisibleSize();

    _client = SocketClient::getInstance();

    if (_client->connected()) {

        auto bg = Sprite::createWithSpriteFrameName(Variables::BG1);

        bg->setScale((_visibleSize.width / 3) / bg->getContentSize().width,
                     (_visibleSize.height / 3) / bg->getContentSize().height);

        bg->setPosition(_visibleSize.width / 2, 4 * _visibleSize.height / 5 - bg->getBoundingBox().size.height / 2);

        this->addChild(bg, 1);

        auto editBoxSize = Size(3 * bg->getBoundingBox().size.width / 4, bg->getBoundingBox().size.height / 5);

        _editName = ui::EditBox::create(editBoxSize, Variables::BAR, ui::Widget::TextureResType::PLIST);
        _editName->setPosition(Vec2(visibleOrigin.x + _visibleSize.width / 2,
                                    bg->getBoundingBox().getMaxY() - 3 * editBoxSize.height / 2));
        _editName->setFontName(Variables::FONT_NAME.c_str());
        _editName->setFontColor(Color3B::BLACK);
        _editName->setFontSize((int)Variables::FONT_SIZE());
        _editName->setMaxLength(12);
        _editName->setPlaceHolder(LocalizedStrings::getInstance()->getString("NAME"));
        _editName->setPlaceholderFontColor(Color3B::BLACK);
        _editName->setPlaceholderFontSize((int)Variables::FONT_SIZE());
        _editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
        _editName->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        this->addChild(_editName, 3);

        _editPassword = ui::EditBox::create(editBoxSize, Variables::BAR, ui::Widget::TextureResType::PLIST);
        _editPassword->setPosition(Vec2(_editName->getPosition().x,
                                        bg->getBoundingBox().getMaxY() - 7 * editBoxSize.height / 2));
        _editPassword->setFontName(Variables::FONT_NAME.c_str());
        _editPassword->setFontColor(Color3B::BLACK);
        _editPassword->setFontSize((int)Variables::FONT_SIZE());
        _editPassword->setPlaceHolder(LocalizedStrings::getInstance()->getString("PASSWORD"));
        _editPassword->setPlaceholderFontColor(Color3B::BLACK);
        _editPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
        _editPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        _editPassword->setPlaceholderFontSize((int)Variables::FONT_SIZE());
        this->addChild(_editPassword, 3);

        _errorMessage = cocos2d::Label::createWithTTF("", Variables::FONT_NAME, Variables::FONT_SIZE());
        _errorMessage->setPosition(cocos2d::Vec2(_editPassword->getPosition().x, bg->getBoundingBox().getMinY() + Variables::FONT_SIZE() / 2));
        _errorMessage->setTextColor(Color4B::RED);

        this->addChild(_errorMessage, 4);

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
        this->addChild(backButton, 3);

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

        auto login = cocos2d::ui::Button::create();
        login->loadTextures(Variables::GREEN_BUTTON, Variables::GREEN_PRESSED_BUTTON, Variables::GREEN_BUTTON,
                            cocos2d::ui::Widget::TextureResType::PLIST);

        login->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
            switch (type) {
                case cocos2d::ui::Widget::TouchEventType::ENDED: {
                    onMenuClick(3);
                }
                    break;
                default:
                    break;
            }
        });

        login->setPosition(
                Vec2(_errorMessage->getPosition().x,
                     bg->getBoundingBox().getMinY() - login->getBoundingBox().size.height));
        auto login_label = cocos2d::Label::createWithTTF(
                LocalizedStrings::getInstance()->getString("LOGIN"), Variables::FONT_NAME,
                Variables::FONT_SIZE());
        login_label->setPosition(login->getContentSize().width / 2,
                                 login->getContentSize().height / 2);
        login->addChild(login_label, 4);

        this->addChild(login, 3);

        auto registration = cocos2d::ui::Button::create();
        registration->loadTextures(Variables::RED_BUTTON, Variables::RED_PRESSED_BUTTON, Variables::RED_BUTTON,
                                   cocos2d::ui::Widget::TextureResType::PLIST);

        registration->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
            switch (type) {
                case cocos2d::ui::Widget::TouchEventType::ENDED: {
                    onMenuClick(2);
                }
                    break;
                default:
                    break;
            }
        });

        registration->setPosition(
                Vec2(login->getPosition().x,
                     login->getPosition().y - login->getBoundingBox().size.height / 2 -
                     registration->getBoundingBox().size.height / 2 - 15.f));
        auto registration_label = cocos2d::Label::createWithTTF(
                LocalizedStrings::getInstance()->getString("REGISTER"), Variables::FONT_NAME,
                Variables::FONT_SIZE());
        registration_label->setPosition(registration->getContentSize().width / 2,
                                        registration->getContentSize().height / 2);
        registration->addChild(registration_label, 4);

        this->addChild(registration, 3);

    } else {
        auto label = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("CONNECTION ERROR"),
                                                   Variables::FONT_NAME, Variables::FONT_SIZE());
        label->setColor(cocos2d::Color3B::BLACK);
        auto popUp = MainMenuPopUp::create("", label, false);
        popUp->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);
        this->addChild(popUp, 0, "PopUp");
    }

    this->schedule(SEL_SCHEDULE(&MultiplayerMenu::update), 1.f);

    return true;
}

void MultiplayerMenu::onMenuClick(int id) {
    switch (id) {
        case 1: {
            break;
        }
        case 2: {
            MainScene::getInstance()->replaceMain(RegisterMenu::create());
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
                _errorMessage->setString(LocalizedStrings::getInstance()->getString("INPUT YOUR INFO"));
            }
        }
            break;
        default:
            break;
    }
}

void MultiplayerMenu::onQuit() {
    MainScene::getInstance()->popAndReplace();
}

void MultiplayerMenu::onError(string message) {
    _errorMessage->setString(message);
}

void MultiplayerMenu::update(float dt) {
    if (!_client->connected() && this->getChildByName("PopUp") == nullptr) {
        auto label = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("CONNECTION ERROR"),
                                                   Variables::FONT_NAME,
                                                   Variables::FONT_SIZE());
        label->setColor(cocos2d::Color3B::BLACK);
        auto popUp = MainMenuPopUp::create("",
                                           label);
        auto _visibleSize = Director::getInstance()->getVisibleSize();
        popUp->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);
        this->addChild(popUp, 0, "PopUp");
    }
}