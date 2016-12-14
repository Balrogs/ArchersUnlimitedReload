//
// Created by igor on 05.12.16.
//

#include <GameEngine/Global/Variables.h>
#include <Scenes/PlayLayers/Battle.h>
#include <ui/UIButton.h>
#include "PopUp.h"

USING_NS_CC;

PopUp *PopUp::create(std::string title, cocos2d::Node *message, bool isTwoButtons) {
    PopUp *ret = new(std::nothrow) PopUp();
    if (ret && ret->init(title, message, isTwoButtons)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

PopUp *PopUp::create(std::string title, cocos2d::Node *message) {
    PopUp *ret = new(std::nothrow) PopUp();
    if (ret && ret->init(title, message)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool PopUp::init(std::string title) {
    if (!Node::init()) {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    POPUP_SIZE = Size(visibleSize.width / 2, visibleSize.height - 100.f);


    auto black = LayerColor::create(Color4B(0, 0, 0, 160));
    auto pos = Vec2(-Director::getInstance()->getVisibleSize().width / 2,
                    -Director::getInstance()->getVisibleSize().height / 2);
    black->setPosition(pos);
    this->addChild(black);

    auto base = Sprite::createWithSpriteFrameName(Variables::BG2);
    _scale = Vec2(POPUP_SIZE.width / base->getContentSize().width, POPUP_SIZE.height / base->getContentSize().height);
    base->setScale(_scale.x, _scale.y);
    this->addChild(base);

    _title = Label::createWithTTF(title, Variables::FONT_NAME, Variables::H_FONT_SIZE);
    _title->setPosition(0.f, POPUP_SIZE.height / 2 - _title->getContentSize().height / 2 - 20.f);
    _title->setColor(cocos2d::Color3B::BLACK);
    this->addChild(_title, 2);

    return true;
}

bool PopUp::init(std::string title, cocos2d::Node *message) {
    if (!PopUp::init(title)) {
        return false;
    }
    message->setPosition(0, 0);
    this->addChild(message, 2);
    return true;
}

bool PopUp::init(std::string title, cocos2d::Node *message, bool isTwoButtons) {
    if (!PopUp::init(title)) {
        return false;
    }

    message->setPosition(0, 0);
    this->addChild(message, 2);

    if (isTwoButtons) {
        auto yes = ui::Button::create();
        yes->loadTextures(Variables::YES_BUTTON_PATH, Variables::YES_PRESSED_BUTTON_PATH, Variables::YES_BUTTON_PATH,
                          cocos2d::ui::Widget::TextureResType::PLIST);
        yes->setPosition(Vec2(-yes->getBoundingBox().size.width, 0.f));

        yes->addTouchEventListener(CC_CALLBACK_0(PopUp::yesAction, this));

        auto no = ui::Button::create();
        no->loadTextures(Variables::NO_BUTTON_PATH, Variables::NO_PRESSED_BUTTON_PATH, Variables::NO_BUTTON_PATH,
                         cocos2d::ui::Widget::TextureResType::PLIST);
        no->setPosition(Vec2(no->getBoundingBox().size.width, 0.f));
        no->addTouchEventListener(CC_CALLBACK_0(PopUp::noAction, this));

        _buttons = Node::create();
        _buttons->addChild(yes);
        _buttons->addChild(no);
        _buttons->setPosition(0.f, -POPUP_SIZE.height / 2 + yes->getBoundingBox().size.height / 2 + 25.f);

    } else {
        auto ok = ui::Button::create();
        ok->loadTextures(Variables::YES_BUTTON_PATH, Variables::YES_PRESSED_BUTTON_PATH, Variables::YES_BUTTON_PATH,
                         cocos2d::ui::Widget::TextureResType::PLIST);
        ok->addTouchEventListener(CC_CALLBACK_0(PopUp::okAction, this));

        _buttons = Node::create();
        _buttons->addChild(ok);
        _buttons->setPosition(0, -POPUP_SIZE.height / 2 + ok->getBoundingBox().size.height / 2 + 25.f);
    }

    this->addChild(_buttons, 2);
    return true;
}

void PopUp::noAction() {
    this->removeFromParent();
    BattleScene::instance->unPause();
}

void PopUp::yesAction() {
    this->removeFromParent();
    BattleScene::instance->onPopScene();
}

void PopUp::okAction() {
    this->removeFromParent();
    BattleScene::instance->onPopScene();
}

void MainMenuPopUp::noAction() {
    this->getParent()->getEventDispatcher()->resumeEventListenersForTarget(this->getParent(), true);
    this->removeFromParent();
}

void MainMenuPopUp::yesAction() {
    this->removeFromParent();
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

}

void MainMenuPopUp::okAction() {
    this->removeFromParent();
    Director::getInstance()->popScene();
}

MainMenuPopUp *MainMenuPopUp::create(std::string title, cocos2d::Node *message, bool isTwoButtons) {
    MainMenuPopUp *ret = new(std::nothrow) MainMenuPopUp();
    if (ret && ret->init(title, message, isTwoButtons)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

MainMenuPopUp *MainMenuPopUp::create(std::string title, cocos2d::Node *message) {
    MainMenuPopUp *ret = new(std::nothrow) MainMenuPopUp();
    if (ret && ret->init(title, message)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

PausePopUp *PausePopUp::create(std::string title) {
    PausePopUp *ret = new(std::nothrow) PausePopUp();
    if (ret && ret->init(title)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool PausePopUp::init(std::string title) {
    if (!PopUp::init(title)) {
        return false;
    }

    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    _musicState = def->getBoolForKey("MUSIC", true);
    _effectsState = def->getBoolForKey("EFFECTS", true);

    def->flush();

    _reloadButtons();

    auto goToMenu = ui::Button::create();
    goToMenu->loadTextures(Variables::RED_BUTTON, Variables::RED_PRESSED_BUTTON, Variables::RED_BUTTON,
                           cocos2d::ui::Widget::TextureResType::PLIST);
    goToMenu->setPosition(Vec2(0.f, 0.f));

    goToMenu->addTouchEventListener(CC_CALLBACK_0(PausePopUp::yesAction, this));

    auto goToMenuTitle = cocos2d::Label::createWithTTF("GO TO MENU", Variables::FONT_NAME,
                                                       Variables::FONT_SIZE);
    goToMenuTitle->setPosition(goToMenu->getContentSize().width / 2,
                               goToMenu->getContentSize().height / 2);
    goToMenu->addChild(goToMenuTitle, 4);

    auto play = ui::Button::create();
    play->loadTextures(Variables::BLUE_BUTTON, Variables::BLUE_PRESSED_BUTTON, Variables::BLUE_BUTTON,
                       cocos2d::ui::Widget::TextureResType::PLIST);
    play->setPosition(Vec2(0.f, goToMenu->getPosition().y - 3 * goToMenu->getBoundingBox().size.height / 2));
    play->addTouchEventListener(CC_CALLBACK_0(PausePopUp::noAction, this));

    auto playTitle = cocos2d::Label::createWithTTF("CONTINUE", Variables::FONT_NAME,
                                                   Variables::FONT_SIZE);
    playTitle->setPosition(play->getContentSize().width / 2,
                           play->getContentSize().height / 2);
    play->addChild(playTitle, 4);

    _buttons = Node::create();
    _buttons->addChild(goToMenu);
    _buttons->addChild(play);
    _buttons->setPosition(0, _musicButton->getPosition().y - goToMenu->getBoundingBox().size.height - 10.f);
    this->addChild(_buttons, 2);

    return true;
}

void PausePopUp::_reloadButtons() {
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

    _musicButton->setPosition(Vec2(-_musicButton->getBoundingBox().size.width,
                                   _title->getPosition().y - _musicButton->getContentSize().height));

    this->addChild(_musicButton);

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
    _effectsButton->setPosition(Vec2(_effectsButton->getBoundingBox().size.width, _musicButton->getPosition().y));
    this->addChild(_effectsButton);
}

void PausePopUp::noAction() {
    PopUp::noAction();
}

void PausePopUp::yesAction() {
    PopUp::yesAction();
}
