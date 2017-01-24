//
// Created by igor on 04.10.16.
//

#include <GameEngine/Global/Misc/Views.h>
#include <GameEngine/Global/Misc/JSONParser.h>
#include <Scenes/PlayLayers/Duel/DuelSceneMultiplayer.h>
#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/PopUp.h>
#include <Localization/LocalizedStrings.h>
#include "MainMenu.h"
#include "Settings.h"

USING_NS_CC;

MainScene *MainScene::create() {
    MainScene *ret = new(std::nothrow) MainScene();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MainScene::init() {
    if (!Scene::init()) {
        return false;
    }

    _backgroundLayer = BackgroundLayer::create();
    _equipmentScene = EquipmentScene::create();

    this->addChild(_backgroundLayer, 2);

    this->addChild(_equipmentScene, 3);

    _main = MainMenu::create(_equipmentScene);

    this->addChild(_main, 4);

    return true;
}

void MainScene::replaceMain(Layer *layer) {
    this->removeChild(_main);
    _main = layer;
    this->addChild(_main, 4);
}

EquipmentScene *MainScene::getEquipmentLayer() {
    return _equipmentScene;
}

MainMenu *MainMenu::create(EquipmentScene *equipmentLayer) {
    MainMenu *ret = new(std::nothrow) MainMenu();
    if (ret && ret->init(equipmentLayer)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;

}

bool MainMenu::init(EquipmentScene *equipmentLayer) {
    if (!Layer::init()) {
        return false;
    }

    _visibleSize = Director::getInstance()->getVisibleSize();

    _menu = Node::create();
    onMenuClick(0);
    this->addChild(_menu);

    auto settingsButton = cocos2d::ui::Button::create();
    settingsButton->loadTextures(Variables::SETTINGS_BUTTON, Variables::SETTINGS_PRESSED_BUTTON,
                                 Variables::SETTINGS_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);

    settingsButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                ((MainScene *) this->getParent())->replaceMain(Settings::create());
            }
                break;
            default:
                break;
        }
    });
    settingsButton->setPosition(Vec2(_visibleSize.width - settingsButton->getBoundingBox().size.width / 2 - 15.f,
                                     settingsButton->getBoundingBox().size.height / 2 + 15.f));
    this->addChild(settingsButton);


    _keyboardListener = cocos2d::EventListenerKeyboard::create();
    _keyboardListener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
        switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_BREAK:
            case EventKeyboard::KeyCode::KEY_ESCAPE:
            case EventKeyboard::KeyCode::KEY_BACKSPACE: {
                if (_menuId == 0) {
                    auto popUp = this->getChildByName("PopUp");
                    if (popUp == nullptr) {

                        this->getEventDispatcher()->pauseEventListenersForTarget(this);

                        auto size = Director::getInstance()->getVisibleSize();

                        auto label = cocos2d::Label::createWithTTF(
                                LocalizedStrings::getInstance()->getString("EXIT THE GAME?"), Variables::FONT_NAME,
                                Variables::FONT_SIZE);
                        label->setColor(cocos2d::Color3B::BLACK);
                        popUp = MainMenuPopUp::create(LocalizedStrings::getInstance()->getString("ARE YOU SURE?"),
                                                      label,
                                                      true);

                        popUp->setPosition(size.width / 2, size.height / 2);
                        this->addChild(popUp, 0, "PopUp");
                    } else {
                        popUp->removeFromParent();
                    }
                } else {
                    onMenuClick(0);
                }
            }
                break;
            default:
                break;
        }
    };


    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    auto coins_bar = cocos2d::ui::Button::create();
    coins_bar->loadTextureNormal(Variables::COIN_BAR, cocos2d::ui::Widget::TextureResType::PLIST);

    coins_bar->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                //TODO add action
            }
                break;
            default:
                break;
        }
    });
    coins_bar->setScale(0.5f);
    coins_bar->setPosition(Vec2(_visibleSize.width - coins_bar->getBoundingBox().size.width / 2 - 15.f,
                                _visibleSize.height - coins_bar->getBoundingBox().size.height / 2 - 15.f));

    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    auto coins = def->getIntegerForKey("COINS", 0);
    _coinsCount = cocos2d::Label::createWithTTF(StringUtils::toString(coins), Variables::FONT_NAME,
                                                    Variables::FONT_SIZE, Size(3 * coins_bar->getContentSize().width / 5, Variables::FONT_SIZE));
    _coinsCount->setHorizontalAlignment(TextHAlignment::RIGHT);
    _coinsCount->setColor(Color3B::WHITE);
    _coinsCount->setAnchorPoint(Vec2(0, 0));
    _coinsCount->setPosition(15.f, coins_bar->getContentSize().height / 2 - Variables::FONT_SIZE / 2);

    coins_bar->addChild(_coinsCount, 5);

    this->addChild(coins_bar);

    auto chest = cocos2d::ui::Button::create();
    chest->loadTextures(Variables::CHEST_BUTTON, Variables::CHEST_PRESSED_BUTTON, Variables::CHEST_BUTTON,
                        cocos2d::ui::Widget::TextureResType::PLIST);

    chest->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                //TODO add action
            }
                break;
            default:
                break;
        }
    });
    chest->setScale(0.6f);
    chest->setPosition(Vec2(chest->getBoundingBox().size.width / 2 + 15.f,
                            _visibleSize.height - chest->getBoundingBox().size.height / 2 - 15.f));

    this->addChild(chest);

    auto customize = cocos2d::ui::Button::create();
    customize->loadTextureNormal(Variables::CUSTOMIZE_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);

    customize->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                //TODO add action
            }
                break;
            default:
                break;
        }
    });

    auto pos = equipmentLayer->getButtonPosition();
    customize->setPosition(Vec2(pos.x, pos.y + customize->getBoundingBox().size.height));

    auto customize_label = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("CUSTOMIZE"),
                                                         Variables::FONT_NAME,
                                                         Variables::FONT_SIZE);
    customize_label->setPosition(customize_label->getBoundingBox().size.width / 2 + 10.f,
                                 customize->getContentSize().height / 2);
    customize->addChild(customize_label, 4);

    this->addChild(customize);

    return true;
}

void MainMenu::onEnter() {
    Layer::onEnter();

    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();
    auto coins = def->getIntegerForKey("COINS", 0);
    _coinsCount->setString(StringUtils::toString(coins));

    SocketClient::destroyInstance();
}

void MainMenu::onPushScene(int id) {
    auto scene = BattleParent::createScene(id);
    Director::getInstance()->pushScene(scene);
}

void MainMenu::onMenuClick(int id) {
    _menu->removeAllChildren();
    _menuId = id;
    switch (id) {
        case 0 : {
            auto singleP = cocos2d::ui::Button::create();
            singleP->loadTextures(Variables::GREEN_BUTTON, Variables::GREEN_PRESSED_BUTTON, Variables::GREEN_BUTTON,
                                  cocos2d::ui::Widget::TextureResType::PLIST);

            singleP->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
                switch (type) {
                    case cocos2d::ui::Widget::TouchEventType::ENDED: {
                        onMenuClick(1);
                    }
                        break;
                    default:
                        break;
                }
            });

            singleP->setPosition(
                    Vec2(_visibleSize.width - singleP->getBoundingBox().size.width / 2 - 30.f,
                         3 * _visibleSize.height / 4));
            auto singleP_label = cocos2d::Label::createWithTTF(
                    LocalizedStrings::getInstance()->getString("SINGLE PLAYER"), Variables::FONT_NAME,
                    Variables::FONT_SIZE);
            singleP_label->setPosition(singleP->getContentSize().width / 2,
                                       singleP->getContentSize().height / 2);
            singleP->addChild(singleP_label, 4);

            _menu->addChild(singleP);

            auto duel2P = cocos2d::ui::Button::create();
            duel2P->loadTextures(Variables::GREEN_BUTTON, Variables::GREEN_PRESSED_BUTTON, Variables::GREEN_BUTTON,
                                 cocos2d::ui::Widget::TextureResType::PLIST);

            duel2P->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
                switch (type) {
                    case cocos2d::ui::Widget::TouchEventType::ENDED: {
                        onPushScene(3);
                    }
                        break;
                    default:
                        break;
                }
            });

            duel2P->setPosition(
                    Vec2(singleP->getPosition().x,
                         singleP->getPosition().y - singleP->getBoundingBox().size.height / 2 -
                         duel2P->getBoundingBox().size.height / 2 - 15.f));
            auto duel2P_label = cocos2d::Label::createWithTTF(
                    LocalizedStrings::getInstance()->getString("DUEL 2 PLAYERS"), Variables::FONT_NAME,
                    Variables::FONT_SIZE);
            duel2P_label->setPosition(duel2P->getContentSize().width / 2,
                                      duel2P->getContentSize().height / 2);
            duel2P->addChild(duel2P_label, 4);


            _menu->addChild(duel2P);

            auto multiP = cocos2d::ui::Button::create();
            multiP->loadTextures(Variables::BLUE_BUTTON, Variables::BLUE_PRESSED_BUTTON, Variables::BLUE_BUTTON,
                                 cocos2d::ui::Widget::TextureResType::PLIST);

            multiP->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
                switch (type) {
                    case cocos2d::ui::Widget::TouchEventType::ENDED: {
                        auto scene = MultiplayerMainMenu::createScene();
                        Director::getInstance()->pushScene(scene);
                    }
                        break;
                    default:
                        break;
                }
            });

            multiP->setPosition(
                    Vec2(duel2P->getPosition().x, duel2P->getPosition().y - duel2P->getBoundingBox().size.height / 2 -
                                                  multiP->getBoundingBox().size.height / 2 - 15.f));
            auto multiP_label = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("MULTIPLAYER"),
                                                              Variables::FONT_NAME,
                                                              Variables::FONT_SIZE);
            multiP_label->setPosition(multiP->getContentSize().width / 2,
                                      multiP->getContentSize().height / 2);
            multiP->addChild(multiP_label, 4);

            _menu->addChild(multiP);

            break;
        }
        case 1 : {

//            auto singleP = cocos2d::ui::Button::create();
//            singleP->loadTextures(Variables::GREEN_BUTTON, Variables::GREEN_PRESSED_BUTTON, Variables::GREEN_BUTTON,
//                                  cocos2d::ui::Widget::TextureResType::PLIST);
//
//            singleP->setEnabled(false);
//
//            singleP->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
//                switch (type) {
//                    case cocos2d::ui::Widget::TouchEventType::ENDED: {
//                        //onPushScene(0);
//                    }
//                        break;
//                    default:
//                        break;
//                }
//            });
//
//            singleP->setPosition(
//                    Vec2(_visibleSize.width - singleP->getBoundingBox().size.width / 2 - 30.f,
//                         3 * _visibleSize.height / 4));
//            auto singleP_label = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("WAVES"),
//                                                               Variables::FONT_NAME,
//                                                               Variables::FONT_SIZE);
//            singleP_label->setPosition(singleP->getContentSize().width / 2,
//                                       singleP->getContentSize().height / 2);
//            singleP->addChild(singleP_label, 4);
//
//
//            _menu->addChild(singleP);

            auto duel = cocos2d::ui::Button::create();
            duel->loadTextures(Variables::GREEN_BUTTON, Variables::GREEN_PRESSED_BUTTON, Variables::GREEN_BUTTON,
                               cocos2d::ui::Widget::TextureResType::PLIST);

            duel->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
                switch (type) {
                    case cocos2d::ui::Widget::TouchEventType::ENDED: {
                        onPushScene(2);
                    }
                        break;
                    default:
                        break;
                }
            });

            duel->setPosition(
                    Vec2(_visibleSize.width - duel->getBoundingBox().size.width / 2 - 30.f,
                         3 * _visibleSize.height / 4));
//                    Vec2(singleP->getPosition().x,
//                         singleP->getPosition().y - singleP->getBoundingBox().size.height / 2 -
//                         duel->getBoundingBox().size.height / 2 - 15.f));
            auto duel_label = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("DUEL"),
                                                            Variables::FONT_NAME,
                                                            Variables::FONT_SIZE);
            duel_label->setPosition(duel->getContentSize().width / 2,
                                    duel->getContentSize().height / 2);
            duel->addChild(duel_label, 4);


            _menu->addChild(duel);

            auto appleB = cocos2d::ui::Button::create();
            appleB->loadTextures(Variables::BLUE_BUTTON, Variables::BLUE_PRESSED_BUTTON, Variables::BLUE_BUTTON,
                                 cocos2d::ui::Widget::TextureResType::PLIST);

            appleB->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
                switch (type) {
                    case cocos2d::ui::Widget::TouchEventType::ENDED: {
                        onPushScene(1);
                    }
                        break;
                    default:
                        break;
                }
            });

            appleB->setPosition(
                    Vec2(duel->getPosition().x, duel->getPosition().y - duel->getBoundingBox().size.height / 2 -
                                                appleB->getBoundingBox().size.height / 2 - 15.f));
            auto appleB_label = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("APPLE"),
                                                              Variables::FONT_NAME,
                                                              Variables::FONT_SIZE);
            appleB_label->setPosition(appleB->getContentSize().width / 2,
                                      appleB->getContentSize().height / 2);
            appleB->addChild(appleB_label, 4);


            _menu->addChild(appleB);

            auto backButton = cocos2d::ui::Button::create();
            backButton->loadTextures(Variables::BACK_BUTTON_PATH, Variables::BACK_PRESSED_BUTTON_PATH,
                                     Variables::BACK_BUTTON_PATH, cocos2d::ui::Widget::TextureResType::PLIST);

            backButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
                switch (type) {
                    case cocos2d::ui::Widget::TouchEventType::ENDED: {
                        onMenuClick(0);
                    }
                        break;
                    default:
                        break;
                }
            });
            backButton->setPosition(
                    Vec2(appleB->getPosition().x, appleB->getPosition().y - appleB->getBoundingBox().size.height / 2 -
                                                  backButton->getBoundingBox().size.height / 2 - 15.f));
            _menu->addChild(backButton);

            break;
        }

        default:
            break;
    }
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
                auto scene = BattleParent::createScene(4);
                Director::getInstance()->pushScene(scene);

                if (auto gameScene = dynamic_cast<DuelSceneMultiplayer *>(BattleParent::getInstance())) {
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
