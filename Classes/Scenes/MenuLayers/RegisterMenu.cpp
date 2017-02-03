
#include <ui/UIEditBox/UIEditBox.h>
#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/SocketClient.h>
#include "RegisterMenu.h"
#include "MultiplayerMenu.h"

USING_NS_CC;


bool RegisterMenu::init() {

    if(!Layer::init()){
        return false;
    }
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

    return true;
}

void RegisterMenu::onEnter() {
    Layer::onEnter();
}

void RegisterMenu::onQuit() {
    this->getParent()->addChild(MultiplayerMenu::getInstance());
    this->removeFromParent();
}