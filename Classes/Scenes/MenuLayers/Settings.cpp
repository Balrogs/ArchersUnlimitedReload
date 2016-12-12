//
// Created by igor on 13.10.16.
//

#include <Scenes/Layers/BackgroundLayer.h>
#include <ui/UIButton.h>
#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/PopUp.h>
#include "Settings.h"
#include "MainMenu.h"

USING_NS_CC;

bool Settings::init() {
    if (!Layer::init()) {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _bg = Sprite::createWithSpriteFrameName(Variables::BG1);

    _bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    this->addChild(_bg, 1);

    //TODO add language selection

    //TODO add rate&review button

    auto title = cocos2d::Label::createWithTTF("SETTINGS", Variables::FONT_NAME,
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
    auto ads_title = cocos2d::Label::createWithTTF("REMOVE ADS", Variables::FONT_NAME,
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

    auto music = cocos2d::Label::createWithTTF("MUSIC:", Variables::FONT_NAME,
                                               Variables::FONT_SIZE);
    music->setPosition(
            Vec2(_musicButton->getBoundingBox().getMinX() - music->getBoundingBox().size.width / 2 - 50.f,
                 _musicButton->getPosition().y));

    this->addChild(music, 2);

    auto sounds = cocos2d::Label::createWithTTF("SOUNDS:", Variables::FONT_NAME,
                                                Variables::FONT_SIZE);
    sounds->setPosition(
            Vec2(_effectsButton->getBoundingBox().getMinX() - sounds->getBoundingBox().size.width / 2 - 50.f,
                 _effectsButton->getPosition().y));


    this->addChild(sounds, 2);

    auto language = cocos2d::Label::createWithTTF("LANGUAGE", Variables::FONT_NAME,
                                                  Variables::FONT_SIZE);
    language->setPosition(_bg->getBoundingBox().getMinX() + 150.f + language->getContentSize().width / 2,
                          _bg->getBoundingBox().size.height / 2 + 50.f);

    this->addChild(language, 2);


    return true;
}

void Settings::onEnter() {
    Layer::onEnter();
}

void Settings::onQuit() {
    this->getParent()->addChild(MainMenu::create(), 4);
    this->removeFromParent();
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
                 _bg->getBoundingBox().size.height / 2 + _musicButton->getBoundingBox().size.height / 2 + 100.f));
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

