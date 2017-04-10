
#include <ui/UIEditBox/UIEditBox.h>
#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/SocketClient.h>
#include <Localization/LocalizedStrings.h>
#include "RegisterMenu.h"
#include "MultiplayerMenu.h"
#include "Scenes/MenuLayers/Main/MainMenu.h"

USING_NS_CC;

static std::vector<std::string> _region_list = {"EUROPE",
                                        "ASIA",
                                        "AFRICA",
                                        "NORTH AMERICA",
                                        "SOUTH AMERICA",
                                        "AUSTRALIA",
                                        "OTHER"
};

bool RegisterMenu::init() {

    if(!Layer::init()){
        return false;
    }
    auto editBoxSize = Size(300.f, 50.f);

    auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _bg = Sprite::createWithSpriteFrameName(Variables::BG1);

    _bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    _bg->setScale((visibleSize.width / 2) / _bg->getContentSize().width,
                  (visibleSize.height - 50.f) / _bg->getContentSize().height);

    this->addChild(_bg, 1);

    auto title = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("REGISTRATION"), Variables::FONT_NAME,
                                               Variables::H_FONT_SIZE());
    title->setPosition(_bg->getBoundingBox().size.width / 2 + _bg->getBoundingBox().getMinX(),
                       _bg->getBoundingBox().getMaxY() - title->getContentSize().height / 2 - 50.f);
    this->addChild(title, 2);


    //TODO FIX BOXES

    _editName = ui::EditBox::create(editBoxSize, Variables::BAR, ui::Widget::TextureResType::PLIST);
    _editName->setPosition(Vec2(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 3 / 4));
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
    _editPassword->setPosition(Vec2(_editName->getPosition().x, _editName->getPosition().y -  2 *_editPassword->getBoundingBox().size.height));
    _editPassword->setFontName(Variables::FONT_NAME.c_str());
    _editPassword->setFontColor(Color3B::BLACK);
    _editPassword->setFontSize((int)Variables::FONT_SIZE());
    _editPassword->setPlaceHolder(LocalizedStrings::getInstance()->getString("PASSWORD"));
    _editPassword->setPlaceholderFontColor(Color3B::BLACK);
    _editPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editPassword->setPlaceholderFontSize((int)Variables::FONT_SIZE());
    this->addChild(_editPassword, 3);

    _region = LocalizedStrings::getInstance()->getString("EUROPE");

    _regionBox = cocos2d::ui::Button::create();
    _regionBox->loadTextureNormal(Variables::GRAY_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);
    _regionBox->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                _showScrollView();
            }
                break;
            default:
                break;
        }
    });
    _regionBox->setPosition(cocos2d::Vec2(_editPassword->getPosition().x,
                                          _editPassword->getPosition().y - 5 * _regionBox->getBoundingBox().size.height / 4));
    _regionBox_title = cocos2d::Label::createWithTTF(_region.c_str(), Variables::FONT_NAME,
                                                         Variables::FONT_SIZE());
    _regionBox_title->setColor(Color3B::BLACK);
    _regionBox_title->setPosition(_regionBox->getContentSize().width / 2,
                                 _regionBox->getContentSize().height / 2);
    _regionBox->addChild(_regionBox_title, 4);

    this->addChild(_regionBox, 3);

    _errorMessage = cocos2d::Label::createWithTTF("", Variables::FONT_NAME, Variables::FONT_SIZE());
    _errorMessage->setPosition(cocos2d::Vec2(_regionBox->getPosition().x, _regionBox->getPosition().y - 2 * Variables::FONT_SIZE()));
    _errorMessage->setTextColor(Color4B::RED);
    this->addChild(_errorMessage, 2);


    auto acceptButton = cocos2d::ui::Button::create();
    acceptButton->loadTextures(Variables::BUTTON_PATH, Variables::PRESSED_BUTTON_PATH,
                               Variables::BUTTON_PATH, cocos2d::ui::Widget::TextureResType::PLIST);

    acceptButton->addTouchEventListener([&, _region_list](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                auto name = string(_editName->getText());
                auto password = string(_editPassword->getText());
                ptrdiff_t reg = find(_region_list.begin(), _region_list.end(), _region) - _region_list.begin();
                if(reg >= _region_list.size()) {
                    reg = _region_list.size() - 1;
                }
                if (!name.empty() && !password.empty()) {
                    _client->registerUser(name, reg, password);
                } else {
                    _errorMessage->setString(LocalizedStrings::getInstance()->getString("INPUT YOUR NAME AND PASSWORD"));
                }
            }
                break;
            default:
                break;
        }
    });
    acceptButton->setPosition(Vec2(visibleSize.width / 2 - acceptButton->getBoundingBox().size.width,
                                   _errorMessage->getPosition().y - acceptButton->getBoundingBox().size.height));
    this->addChild(acceptButton, 3);

    _client = SocketClient::getInstance();

    auto backButton = cocos2d::ui::Button::create();
    backButton->loadTextures(Variables::CLOSE_BUTTON_PATH, Variables::CLOSE_PRESSED_BUTTON_PATH,
                             Variables::CLOSE_BUTTON_PATH, cocos2d::ui::Widget::TextureResType::PLIST);

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
    backButton->setPosition(Vec2(visibleSize.width / 2 + backButton->getBoundingBox().size.width,
                                 acceptButton->getPosition().y));
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

    return true;
}

void RegisterMenu::onEnter() {
    Layer::onEnter();
}

void RegisterMenu::onQuit() {
    MainScene::getInstance()->replaceMain(MultiplayerMenu::create());
}

void RegisterMenu::_showScrollView() {
    _scrollView = cocos2d::ui::ScrollView::create();
    _scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    _scrollView->setContentSize(Size(_regionBox->getContentSize().width, _bg->getContentSize().height * 0.8f));
    _scrollView->setInnerContainerSize(
            Size(_regionBox->getContentSize().width, _regionBox->getContentSize().height * 10));
    _scrollView->setBackGroundImage(Variables::GRAY_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);
    _scrollView->setBackGroundImageScale9Enabled(true);
    _scrollView->setBounceEnabled(true);
    _scrollView->setInertiaScrollEnabled(true);
    _scrollView->setAnchorPoint(Vec2(0, 0));
    _scrollView->setPosition(Vec2(_regionBox->getPosition().x - _scrollView->getContentSize().width / 2,
                                  Director::getInstance()->getVisibleSize().height / 2 -
                                  _scrollView->getContentSize().height / 2));

    for (unsigned long i = 0; i < _region_list.size(); i++) {
        auto region = _region_list.at(i);
        auto regionButton = cocos2d::ui::Button::create();
        regionButton->setTitleText(region);
        regionButton->setTitleFontSize(Variables::FONT_SIZE());
        regionButton->setTitleFontName(Variables::FONT_NAME);
        regionButton->setColor(Color3B::BLACK);
        regionButton->addTouchEventListener(
                [&, region](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
                    switch (type) {
                        case cocos2d::ui::Widget::TouchEventType::ENDED: {
                            _regionBox_title->setString(region);
                            this->_region = region;
                            _scrollView->removeFromParent();
                            break;
                        }
                        default:
                            break;
                    }
                });
        regionButton->setPosition(Vec2(_scrollView->getInnerContainerSize().width / 2,
                                       _scrollView->getInnerContainerSize().height - 25.f -
                                       i * _regionBox->getContentSize().height));
        _scrollView->addChild(regionButton, 3);

    }
    this->addChild(_scrollView, 4);
}

void RegisterMenu::onError(string message) {
    _errorMessage->setString(message);
    _editPassword->setText("");
}
