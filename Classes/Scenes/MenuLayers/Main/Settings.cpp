#include <Scenes/Layers/BackgroundLayer.h>
#include <ui/UIButton.h>
#include <GameEngine/Global/Variables.h>
#include <Localization/LocalizedStrings.h>
#include "Settings.h"
#include "Scenes/MenuLayers/Main/MainMenu.h"

USING_NS_CC;

bool Settings::init() {
    if (!Layer::init()) {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();

    _bg = Sprite::createWithSpriteFrameName(Variables::BG1);

    _bg->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);

    _bg->setScale((_visibleSize.width - 50.f) / _bg->getContentSize().width,
                  (_visibleSize.height - 50.f) / _bg->getContentSize().height);

    this->addChild(_bg, 1);

    auto title = cocos2d::Label::createWithTTF( LocalizedStrings::getInstance()->getString("SETTINGS"), Variables::FONT_NAME,
                                               Variables::H_FONT_SIZE());
    title->setPosition(_bg->getBoundingBox().getMinX() + 50.f + title->getContentSize().width / 2,
                       _bg->getBoundingBox().getMaxY() - title->getContentSize().height / 2 - 50.f);
    this->addChild(title, 2);

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
    backButton->setPosition(Vec2(_visibleSize.width / 2,
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
                                               Variables::FONT_SIZE());
    music->setPosition(
            Vec2(_musicButton->getBoundingBox().getMinX() - music->getBoundingBox().size.width / 2 - 50.f,
                 _musicButton->getPosition().y));

    this->addChild(music, 2);

    auto sounds = cocos2d::Label::createWithTTF(  LocalizedStrings::getInstance()->getString("SOUNDS"), Variables::FONT_NAME,
                                                Variables::FONT_SIZE());
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
                 _bg->getPositionY()));
    auto languageBox_title = cocos2d::Label::createWithTTF(_language.c_str(), Variables::FONT_NAME,
                                                           Variables::FONT_SIZE());
    languageBox_title->setColor(Color3B::BLACK);
    languageBox_title->setPosition(_languageBox->getContentSize().width / 2,
                                   _languageBox->getContentSize().height / 2);
    _languageBox->addChild(languageBox_title, 4);

    this->addChild(_languageBox, 3);

    auto language = cocos2d::Label::createWithTTF(  LocalizedStrings::getInstance()->getString("LANGUAGE"), Variables::FONT_NAME,
                                                  Variables::FONT_SIZE());
    language->setPosition(_languageBox->getPosition().x,
                          _languageBox->getPosition().y + _languageBox->getBoundingBox().size.height / 2 +
                          language->getBoundingBox().size.height);

    this->addChild(language, 2);

    this->setPosition(_visibleSize.width, 0);

    return true;
}

void Settings::onEnter() {
    Layer::onEnter();
    this->runAction(Sequence::create(
            CallFunc::create([](){
                MainScene::getInstance()->wait(true);
            }),
            MoveTo::create(0.5f, Vec2::ZERO)
            , NULL)
    );
}

void Settings::onQuit() {
    this->runAction(Sequence::create(
            CallFunc::create([](){
                MainScene::getInstance()->wait(false);
            }),
            MoveTo::create(0.5f, Vec2(_visibleSize.width, 0)),
            CallFunc::create([](){
                MainScene::getInstance()->popAndReplace();
            }), NULL)
    );
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
            Vec2(_bg->getBoundingBox().getMaxX() - 3.f * _musicButton->getBoundingBox().size.width,
                 _bg->getPositionY() - _musicButton->getBoundingBox().size.height));
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
            Vec2(_musicButton->getPositionX(),
                 _bg->getPositionY() + _effectsButton->getBoundingBox().size.height));
    this->addChild(_effectsButton, 3);
}

void Settings::_showScrollView() {

    _scrollView = cocos2d::ui::ScrollView::create();
    _scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    _scrollView->setContentSize(Size(_languageBox->getContentSize().width, _bg->getContentSize().height * 0.8f));
    _scrollView->setInnerContainerSize(
            Size(_languageBox->getContentSize().width, _languageBox->getContentSize().height * 9));
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
        languageButton->setTitleFontSize(Variables::FONT_SIZE());
        languageButton->setTitleFontName(Variables::FONT_NAME);
        languageButton->setColor(Color3B::BLACK);
        languageButton->addTouchEventListener(
                [&, language](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
                    switch (type) {
                        case cocos2d::ui::Widget::TouchEventType::ENDED: {
                            cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();
                            def->setStringForKey("LANGUAGE", language);
                            LocalizedStrings::getInstance()->setLanguage(language);
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

