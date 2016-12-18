//
// Created by igor on 13.10.16.
//

#include <Scenes/Layers/BackgroundLayer.h>
#include <ui/UIButton.h>
#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/PopUp.h>
#include <Localization/LocalizedStrings.h>
#include "Settings.h"
#include "MainMenu.h"

USING_NS_CC;

bool Settings::init() {
    if (!Layer::init()) {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto black = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(black);

    _bg = Sprite::createWithSpriteFrameName(Variables::BG1);

    _bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    _bg->setScale((visibleSize.width - 50.f) / _bg->getContentSize().width,
                  (visibleSize.height - 50.f) / _bg->getContentSize().height);

    this->addChild(_bg, 1);

    auto title = cocos2d::Label::createWithTTF( LocalizedStrings::getInstance()->getString("SETTINGS"), Variables::FONT_NAME,
                                               Variables::H_FONT_SIZE);
    title->setPosition(_bg->getBoundingBox().getMinX() + 50.f + title->getContentSize().width / 2,
                       _bg->getBoundingBox().getMaxY() - title->getContentSize().height / 2 - 50.f);
    this->addChild(title, 2);


    auto removeAds = cocos2d::ui::Button::create();
    removeAds->loadTextures(Variables::REMOVE_ADS_BUTTON, Variables::REMOVE_ADS_PRESSED_BUTTON,
                            Variables::REMOVE_ADS_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);

    removeAds->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                //TODO add action here
            }
                break;
            default:
                break;
        }
    });
    removeAds->setPosition(Vec2(_bg->getBoundingBox().getMaxX() * 7 / 8 - removeAds->getBoundingBox().size.width / 2,
                                title->getPosition().y));
    auto ads_title = cocos2d::Label::createWithTTF(  LocalizedStrings::getInstance()->getString("REMOVE ADS"), Variables::FONT_NAME,
                                                   Variables::FONT_SIZE);
    ads_title->setPosition(removeAds->getContentSize().width - ads_title->getContentSize().width / 2 - 25.f,
                           removeAds->getContentSize().height / 2);
    removeAds->addChild(ads_title, 4);

    this->addChild(removeAds, 3);


    auto backButton = cocos2d::ui::Button::create();
    backButton->loadTextures(Variables::BUTTON_PATH, Variables::PRESSED_BUTTON_PATH,
                             Variables::BUTTON_PATH, cocos2d::ui::Widget::TextureResType::PLIST);

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
    backButton->setPosition(Vec2(visibleSize.width / 2,
                                 backButton->getBoundingBox().size.height / 2 + _bg->getBoundingBox().getMinY() +
                                 25.f));
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

    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    _musicState = def->getBoolForKey("MUSIC", true);
    _effectsState = def->getBoolForKey("EFFECTS", true);

    def->flush();

    _reloadButtons();

    auto music = cocos2d::Label::createWithTTF(  LocalizedStrings::getInstance()->getString("MUSIC"), Variables::FONT_NAME,
                                               Variables::FONT_SIZE);
    music->setPosition(
            Vec2(_musicButton->getBoundingBox().getMinX() - music->getBoundingBox().size.width / 2 - 50.f,
                 _musicButton->getPosition().y));

    this->addChild(music, 2);

    auto sounds = cocos2d::Label::createWithTTF(  LocalizedStrings::getInstance()->getString("SOUNDS"), Variables::FONT_NAME,
                                                Variables::FONT_SIZE);
    sounds->setPosition(
            Vec2(_effectsButton->getBoundingBox().getMinX() - sounds->getBoundingBox().size.width / 2 - 50.f,
                 _effectsButton->getPosition().y));


    this->addChild(sounds, 2);


    _language = def->getStringForKey("LANGUAGE", "ENGLISH");

    _languageBox = cocos2d::ui::Button::create();
    _languageBox->loadTextureNormal(Variables::GRAY_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);

    _languageBox->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                _showScrollView();
            }
                break;
            default:
                break;
        }
    });
    _languageBox->setPosition(
            Vec2(_bg->getBoundingBox().getMinX() + 150.f + _languageBox->getBoundingBox().size.width / 2,
                 _bg->getBoundingBox().size.height / 2 + _languageBox->getContentSize().height));
    auto languageBox_title = cocos2d::Label::createWithTTF(_language.c_str(), Variables::FONT_NAME,
                                                           Variables::FONT_SIZE);
    languageBox_title->setColor(Color3B::BLACK);
    languageBox_title->setPosition(_languageBox->getContentSize().width / 2,
                                   _languageBox->getContentSize().height / 2);
    _languageBox->addChild(languageBox_title, 4);

    this->addChild(_languageBox, 3);

    auto language = cocos2d::Label::createWithTTF(  LocalizedStrings::getInstance()->getString("LANGUAGE"), Variables::FONT_NAME,
                                                  Variables::FONT_SIZE);
    language->setPosition(_languageBox->getPosition().x,
                          _languageBox->getPosition().y + _languageBox->getBoundingBox().size.height / 2 +
                          language->getBoundingBox().size.height);

    this->addChild(language, 2);

    auto rate = cocos2d::ui::Button::create();
    rate->loadTextures(Variables::GREEN_BUTTON, Variables::GREEN_PRESSED_BUTTON,
                       Variables::GREEN_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);

    rate->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                //TODO add action here
            }
                break;
            default:
                break;
        }
    });

    rate->setPosition(Vec2(_languageBox->getPosition().x,
                           _languageBox->getPosition().y - rate->getBoundingBox().size.height - 15.f));

    auto rate_icon = Sprite::createWithSpriteFrameName(Variables::RATE_ICON);
    rate_icon->setPosition(rate->getContentSize().width - rate_icon->getBoundingBox().size.width / 2 - 15.f,
                           rate->getContentSize().height / 2);
    rate->addChild(rate_icon, 5);

    auto rate_title = cocos2d::Label::createWithTTF(  LocalizedStrings::getInstance()->getString("RATE US"), Variables::FONT_NAME,
                                                    Variables::FONT_SIZE);
    rate_title->setPosition((rate->getContentSize().width / 2 - rate_title->getContentSize().width / 2),
                            rate->getContentSize().height / 2);
    rate->addChild(rate_title, 4);

    this->addChild(rate, 3);


    return true;
}

void Settings::onEnter() {
    Layer::onEnter();
}

void Settings::onQuit() {
    auto scene = (MainScene *) this->getParent();
    scene->replaceMain(MainMenu::create(scene->getEquipmentLayer()));
}

void Settings::_reloadButtons() {
    if (_musicButton != nullptr) {
        _musicButton->removeFromParent();
    }

    if (_effectsButton != nullptr) {
        _effectsButton->removeFromParent();
    }

    string music = Variables::MUSIC_ON_BUTTON;
    string effects = Variables::EFFECTS_ON_BUTTON;

    if (!_musicState) {
        music = Variables::MUSIC_OFF_BUTTON;
    }

    if (!_effectsState) {
        effects = Variables::EFFECTS_OFF_BUTTON;
    }

    _musicButton = cocos2d::ui::Button::create();
    _musicButton->loadTextureNormal(music, cocos2d::ui::Widget::TextureResType::PLIST);

    _musicButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();
                _musicState = !_musicState;
                def->setBoolForKey("MUSIC", _musicState);
                this->_reloadButtons();
            }
                break;
            default:
                break;
        }
    });
    _musicButton->setPosition(
            Vec2(_bg->getBoundingBox().getMaxX() - _musicButton->getBoundingBox().size.width / 2 - 100.f,
                 _bg->getBoundingBox().getMaxY() - 3 * _musicButton->getBoundingBox().size.height));
    this->addChild(_musicButton, 3);

    _effectsButton = cocos2d::ui::Button::create();
    _effectsButton->loadTextureNormal(effects, cocos2d::ui::Widget::TextureResType::PLIST);
    _effectsButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();
                _effectsState = !_effectsState;
                def->setBoolForKey("EFFECTS", _effectsState);
                this->_reloadButtons();
            }
                break;
            default:
                break;
        }
    });
    _effectsButton->setPosition(
            Vec2(_bg->getBoundingBox().getMaxX() - _effectsButton->getBoundingBox().size.width / 2 - 100.f,
                 _bg->getBoundingBox().size.height / 2 + _effectsButton->getBoundingBox().size.height / 2 - 100.f));
    this->addChild(_effectsButton, 3);
}

void Settings::_showScrollView() {

    _scrollView = cocos2d::ui::ScrollView::create();
    _scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    _scrollView->setContentSize(Size(_languageBox->getContentSize().width, _bg->getContentSize().height));
    _scrollView->setInnerContainerSize(
            Size(_languageBox->getContentSize().width, _languageBox->getContentSize().height * 10));
    _scrollView->setBackGroundImage(Variables::GRAY_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);
    _scrollView->setBackGroundImageScale9Enabled(true);
    _scrollView->setBounceEnabled(true);
    _scrollView->setInertiaScrollEnabled(true);
    _scrollView->setAnchorPoint(Vec2(0, 0));
    _scrollView->setPosition(Vec2(_languageBox->getPosition().x - _scrollView->getContentSize().width / 2,
                                  Director::getInstance()->getVisibleSize().height / 2 -
                                  _scrollView->getContentSize().height / 2));

    std::vector<std::string> language_list = {"ENGLISH",
                                              "GERMAN",
                                              "FRENCH",
                                              "ITALIAN",
                                              "SPANISH",
                                              "PORTUGUESE",
                                              "RUSSIAN",
                                              "JAPANESE",
                                              "CHINESE"
    };
    for (unsigned long i = 0; i < language_list.size(); i++) {
        auto language = language_list.at(i);
        auto languageButton = cocos2d::ui::Button::create();
        languageButton->setTitleText(language);
        languageButton->setTitleFontSize(Variables::FONT_SIZE);
        languageButton->setTitleFontName(Variables::FONT_NAME);
        languageButton->setColor(Color3B::BLACK);
        languageButton->addTouchEventListener(
                [&, language](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
                    switch (type) {
                        case cocos2d::ui::Widget::TouchEventType::ENDED: {
                            cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();
                            def->setStringForKey("LANGUAGE", language);
                            onQuit();
                            break;
                        }
                        default:
                            break;
                    }
                });
        languageButton->setPosition(Vec2(_scrollView->getInnerContainerSize().width / 2,
                                         _scrollView->getInnerContainerSize().height - 25.f -
                                         i * _languageBox->getContentSize().height));
        _scrollView->addChild(languageButton, 3);

    }
    this->addChild(_scrollView, 4);
}

