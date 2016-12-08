//
// Created by igor on 05.12.16.
//

#include <GameEngine/Global/Variables.h>
#include <Scenes/PlayLayers/Battle.h>
#include <ui/UIButton.h>
#include "PopUp.h"

USING_NS_CC;

const Size PopUp::POPUP_SIZE = Size(500.f, 200.f);

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

    auto black = LayerColor::create(Color4B(0, 0, 0, 160));
    auto pos = Vec2(-Director::getInstance()->getVisibleSize().width / 2,
                    -Director::getInstance()->getVisibleSize().height / 2);
    black->setPosition(pos);
    this->addChild(black);

    auto base = DrawNode::create();
    base->drawSolidRect(Vec2(-PopUp::POPUP_SIZE.width / 2, -PopUp::POPUP_SIZE.height / 2),
                        Vec2(PopUp::POPUP_SIZE.width / 2, PopUp::POPUP_SIZE.height / 2),
                        Color4F(Color3B(255, 212, 45), 1.f));
    this->addChild(base);

    auto titleBase = DrawNode::create();
    titleBase->drawSolidRect(Vec2(-POPUP_SIZE.width / 2, POPUP_SIZE.height / 2 - 40.f),
                             Vec2(POPUP_SIZE.width / 2, POPUP_SIZE.height / 2), Color4F(Color3B(242, 118, 38), 1.f));
    this->addChild(titleBase, 1);

    _title = Label::createWithTTF(title, Variables::FONT_NAME, Variables::FONT_SIZE);
    _title->setPosition(0.f, POPUP_SIZE.height / 2 - 20.f);
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
        yes->setPosition(Vec2(-30.f, 0.f));

        yes->addTouchEventListener(CC_CALLBACK_0(PopUp::yesAction, this));

        auto no = ui::Button::create();
        no->loadTextures(Variables::NO_BUTTON_PATH, Variables::NO_PRESSED_BUTTON_PATH, Variables::NO_BUTTON_PATH,
                         cocos2d::ui::Widget::TextureResType::PLIST);
        no->setPosition(Vec2(30.f, 0.f));
        no->addTouchEventListener(CC_CALLBACK_0(PopUp::noAction, this));

        _buttons = Node::create();
        _buttons->addChild(yes);
        _buttons->addChild(no);
        _buttons->setPosition(0, -POPUP_SIZE.height / 2 + 30.f);
        this->addChild(_buttons, 2);
    } else {
        auto ok = ui::Button::create();
        ok->loadTextures(Variables::YES_BUTTON_PATH, Variables::YES_PRESSED_BUTTON_PATH, Variables::YES_BUTTON_PATH,
                         cocos2d::ui::Widget::TextureResType::PLIST);
        ok->setPosition(Vec2(0.f, 0.f));
        ok->addTouchEventListener(CC_CALLBACK_0(PopUp::okAction, this));

        _buttons = Node::create();
        _buttons->addChild(ok);
        _buttons->setPosition(0, -POPUP_SIZE.height / 2 + 30.f);
        this->addChild(_buttons, 2);
    }
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

    // TODO add play button

    // TODO add menu button

    auto yes = ui::Button::create();
    yes->loadTextures(Variables::YES_BUTTON_PATH, Variables::YES_PRESSED_BUTTON_PATH, Variables::YES_BUTTON_PATH,
                      cocos2d::ui::Widget::TextureResType::PLIST);
    yes->setPosition(Vec2(-30.f, 0.f));

    yes->addTouchEventListener(CC_CALLBACK_0(PausePopUp::yesAction, this));

    auto no = ui::Button::create();
    no->loadTextures(Variables::NO_BUTTON_PATH, Variables::NO_PRESSED_BUTTON_PATH, Variables::NO_BUTTON_PATH,
                     cocos2d::ui::Widget::TextureResType::PLIST);
    no->setPosition(Vec2(30.f, 0.f));
    no->addTouchEventListener(CC_CALLBACK_0(PausePopUp::noAction, this));

    _buttons = Node::create();
    _buttons->addChild(yes);
    _buttons->addChild(no);
    _buttons->setPosition(0, -POPUP_SIZE.height / 2 + 30.f);
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

    auto visibleSize = Director::getInstance()->getVisibleSize();

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
    _musicButton->setPosition(Vec2(30.f, 20.f));
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
    _effectsButton->setPosition(Vec2(-30.f, 20.f));
    this->addChild(_effectsButton);
}

void PausePopUp::noAction() {
    PopUp::noAction();
}

void PausePopUp::yesAction() {
    PopUp::yesAction();
}
