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

    auto ok = ui::Button::create();
    ok->loadTextures(Variables::YES_BUTTON_PATH, Variables::YES_PRESSED_BUTTON_PATH, Variables::YES_BUTTON_PATH,
                     cocos2d::ui::Widget::TextureResType::PLIST);
    ok->setPosition(Vec2(-30.f, 0.f));
    ok->addTouchEventListener(CC_CALLBACK_0(PopUp::okAction, this));

    _buttons = Node::create();
    _buttons->addChild(ok);
    _buttons->setPosition(0, -POPUP_SIZE.height / 2 + 30.f);
    this->addChild(_buttons, 2);
    return true;
}

bool PopUp::init(std::string title, cocos2d::Node *message, bool isTwoButtons) {
    if (!PopUp::init(title)) {
        return false;
    }

    message->setPosition(0, 0);
    this->addChild(message, 2);

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
    return true;
}

void PopUp::noAction() {
    this->removeFromParent();
    BattleScene::instance->_unPause();
}

void PopUp::yesAction() {
    this->removeFromParent();
    BattleScene::instance->_onPopScene();
}

void PopUp::okAction() {
    this->removeFromParent();
    BattleScene::instance->_onPopScene();
}

void MainMenuPopUp::noAction() {
    this->getParent()->resume();
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
