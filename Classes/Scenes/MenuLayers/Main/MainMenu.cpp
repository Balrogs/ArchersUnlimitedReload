#include <GameEngine/Global/Misc/UI/Views.h>
#include <GameEngine/Global/Misc/JSONParser.h>
#include <Scenes/PlayLayers/Duel/DuelSceneMultiplayer.h>
#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/UI/PopUp.h>
#include <GameEngine/Global/Misc/UI/GiftAlert.h>
#include <Localization/LocalizedStrings.h>
#include "MainMenu.h"
#include "Settings.h"
#include "Scenes/MenuLayers/Multiplayer/MultiplayerMenu.h"
#include "Scenes/MenuLayers/Loading.h"
#include "Randomizer.h"
#include "DailyGiftScene.h"

USING_NS_CC;


MainScene* MainScene::_instance = nullptr;

MainScene *MainScene::getInstance() {
    return _instance;
}

MainScene *MainScene::create() {
    MainScene *ret = new(std::nothrow) MainScene();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    _instance = ret;
    return ret;
}

bool MainScene::init() {
    if (!Scene::initWithPhysics()) {
        return false;
    }

    _backgroundLayer = BackgroundLayer::create();
    _equipmentScene = EquipmentScene::create();
    _equipmentScene->pause();

    this->addChild(_backgroundLayer, 2);

    this->addChild(_equipmentScene, 3);

    _main = MainMenu::create(_equipmentScene);

    pushMain(_main);
    _backgroundLayer->wait(false);

    if(DailyGiftScene::checkDate()) {
        replaceMain(DailyGiftScene::create());
    }

    return true;
}

void MainScene::replaceMain(Layer *layer) {
    while(_mainStack.size()){
        popMain();
    }
    pushMain(layer);
}

void MainScene::pushMain(Layer *layer) {
    _main = layer;
    _mainStack.push(_main);
    this->addChild(_main, 4);

    _backgroundLayer->wait(true);
}

void MainScene::popMain() {
    if (!_mainStack.empty()) {
        this->removeChild(_mainStack.top());
        _mainStack.pop();
    }
}

cocos2d::Layer *MainScene::getMain() {
    return _main;
}

bool MainScene::resumeEquipment() {
    _equipmentScene->resumeEquipment();
}

void MainScene::popAndReplace() {
    while(_mainStack.size()){
        popMain();
    }
    pushMain(MainMenu::create(_equipmentScene));
    _backgroundLayer->wait(false);
}

void MainScene::wait(bool p) {
    _backgroundLayer->wait(p);
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
    _equipmentScene = equipmentLayer;

    _equipmentScene->pause();

    _visibleSize = Director::getInstance()->getVisibleSize();

    _menu = Node::create();
    this->addChild(_menu);
    onMenuClick(0);


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

    cocos2d::Director::getInstance()->getScheduler()->schedule(
            schedule_selector(MainMenu::_enterFrameHandler),
            this, 0.f, false
    );

    _keyboardListener = cocos2d::EventListenerKeyboard::create();
    _keyboardListener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
        switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_BREAK:
            case EventKeyboard::KeyCode::KEY_ESCAPE:
            case EventKeyboard::KeyCode::KEY_BACKSPACE: {
                if (_menuId == 0) {
                    auto popUp = this->getChildByName("PopUp");
                    if (popUp == nullptr) {
                        auto label = cocos2d::Label::createWithTTF(
                                LocalizedStrings::getInstance()->getString("EXIT THE GAME?"), Variables::FONT_NAME,
                                Variables::FONT_SIZE());
                        label->setColor(cocos2d::Color3B::BLACK);
                        popUp = MainMenuPopUp::create(LocalizedStrings::getInstance()->getString("ARE YOU SURE?"),
                                                           label,
                                                           true);
                        showPopUp(popUp);
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
    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();
    auto coins = def->getIntegerForKey("COINS", 0);

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    const auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(MainMenu::_touchHandlerBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    auto coins_bar = cocos2d::ui::Button::create();
    coins_bar->loadTextureNormal(Variables::COIN_BAR, cocos2d::ui::Widget::TextureResType::PLIST);

    coins_bar->addTouchEventListener([&, coins](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                if(coins >= Variables::wheelCost()){
                    MainScene::getInstance()->replaceMain(Randomizer::create());
                }
            }
                break;
            default:
                break;
        }
    });
    coins_bar->setScale(0.5f);
    coins_bar->setPosition(Vec2(_visibleSize.width - coins_bar->getBoundingBox().size.width / 2 - 15.f,
                                _visibleSize.height - coins_bar->getBoundingBox().size.height / 2 - 15.f));




    _coinsCount = cocos2d::Label::createWithTTF(StringUtils::toString(coins), Variables::FONT_NAME,
                                                    Variables::FONT_SIZE(), Size(3 * coins_bar->getContentSize().width / 5, Variables::FONT_SIZE()));
    _coinsCount->setHorizontalAlignment(TextHAlignment::RIGHT);
    _coinsCount->setColor(Color3B::WHITE);
    _coinsCount->setAnchorPoint(Vec2(0, 0));
    _coinsCount->setPosition(15.f, coins_bar->getContentSize().height / 2 - Variables::FONT_SIZE() / 2);

    coins_bar->addChild(_coinsCount, 5);

    this->addChild(coins_bar);

    this->runAction(RepeatForever::create(
            Sequence::create(
                    DelayTime::create(15.f),
                    CallFunc::create([&](){
                        _showHint();
                    }),
                    NULL)));

    return true;
}

void MainMenu::_enterFrameHandler(float passedTime) {
    dragonBones::WorldClock::clock.advanceTime(passedTime);
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
    FiniteTimeAction* action, *postAction;
    if(_menu->getChildrenCount()){
        action = MoveTo::create(0.3f, Vec2(_visibleSize.width / 2, 0));
        postAction = Sequence::create(
                MoveTo::create(0.3f, Vec2::ZERO),
                NULL);
    } else {
        _menu->setPosition(_visibleSize.width / 2, 0);
        action = DelayTime::create(0.f);
        postAction = Sequence::create(
                MoveTo::create(0.75f, Vec2::ZERO),
                NULL);
    }

    auto changeAction = CallFunc::create([&, id]() {
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
                        Variables::FONT_SIZE());
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
                        Variables::FONT_SIZE());
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
                            if (SocketClient::getInstance()->connected() &&
                                SocketClient::getInstance()->getDBPlayer()->canLogin()) {
//                                SocketClient::getInstance()->login();
//                                MainScene::getInstance()->pushMain(Loading::create());
                                MainScene::getInstance()->replaceMain(MultiplayerMenu::create());
                            } else {
                                MainScene::getInstance()->replaceMain(MultiplayerMenu::create());
                            }
                        }
                            break;
                        default:
                            break;
                    }
                });

                multiP->setPosition(
                        Vec2(duel2P->getPosition().x,
                             duel2P->getPosition().y - duel2P->getBoundingBox().size.height / 2 -
                             multiP->getBoundingBox().size.height / 2 - 15.f));
                auto multiP_label = cocos2d::Label::createWithTTF(
                        LocalizedStrings::getInstance()->getString("MULTIPLAYER"),
                        Variables::FONT_NAME,
                        Variables::FONT_SIZE());
                multiP_label->setPosition(multiP->getContentSize().width / 2,
                                          multiP->getContentSize().height / 2);
                multiP->addChild(multiP_label, 4);
                multiP->setEnabled(false);

                auto soon = Sprite::createWithSpriteFrameName(Variables::SOON);
                soon->setPosition(Vec2(
                        multiP->getBoundingBox().size.width - soon->getContentSize().width / 2,
                         soon->getContentSize().height / 2
                ));
                multiP->addChild(soon);
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
//                        //onMenuClick(0);
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
//                                                               Variables::FONT_SIZE());
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
                                                                Variables::FONT_SIZE());
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
                                                                  Variables::FONT_SIZE());
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
                        Vec2(appleB->getPosition().x,
                             appleB->getPosition().y - appleB->getBoundingBox().size.height / 2 -
                             backButton->getBoundingBox().size.height / 2 - 15.f));
                _menu->addChild(backButton);

                break;
            }

            default:
                break;
        }
    });
    _menu->runAction(Sequence::create(action, changeAction, postAction, NULL));

}

void MainMenu::showErrorPopUp() {
    auto label = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("SERVER ERROR"),
                                               Variables::FONT_NAME,
                                               Variables::FONT_SIZE());
    label->setColor(cocos2d::Color3B::BLACK);
    auto popUp = MainMenuPopUp::create("",
                                       label);
    showPopUp(popUp);
}

void MainMenu::_showHint() {
    this->removeChildByName("hint");

    auto hand = Sprite::createWithSpriteFrameName(Variables::HAND);
    auto previewPos = _equipmentScene->getButtonPosition();
    auto startPos = Vec2(previewPos.x * 2.f, previewPos.y * 0.8f);
    hand->setPosition(startPos);
    hand->setScale(0.f);
    this->addChild(hand, 1, "hint");

    hand->runAction(Sequence::create(
            ScaleTo::create(0.8f, 1.f),
            Repeat::create(
                    Sequence::create(
                            MoveTo::create(1.f, previewPos),
                            Repeat::create(
                                    Sequence::create(
                                            ScaleTo::create(0.3f, 0.7f),
                                            ScaleTo::create(0.3f, 1.f),
                                            NULL),2),
                            MoveTo::create(1.f, startPos),
                            NULL),
                    2),
            ScaleTo::create(0.8f, 0.f),
            RemoveSelf::create(),
            NULL
    ));
}

void MainMenu::_removeHint() {
    auto hint = this->getChildByName("hint");
    if(hint != nullptr){
        hint->stopAllActions();
        hint->removeFromParent();
    }
}

bool MainMenu::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if(_equipmentScene->checkTouch(touch->getLocation()) && _equipmentScene->resumeEquipment()){
        MainScene::getInstance()->popMain();
    }
    return true;
}

void MainMenu::showPopUp(Node* popUp) {
    if (this->getChildByName("PopUp") == nullptr) {
        _removeHint();
        auto size = Director::getInstance()->getVisibleSize();

        popUp->setPosition(size.width / 2, -size.height);
        this->addChild(popUp, 0, "PopUp");
    }
}
