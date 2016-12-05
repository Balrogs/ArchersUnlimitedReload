//
// Created by igor on 05.12.16.
//

#include <GameEngine/Global/Variables.h>
#include <Scenes/PlayLayers/Battle.h>
#include "PopUp.h"

USING_NS_CC;

const Size PopUp::POPUP_SIZE = cocos2d::Size(500.f, 200.f);


PopUp::PopUp(std::string title) {

    auto base = DrawNode::create();
    base->drawSolidRect(Vec2(-PopUp::POPUP_SIZE.width / 2, -PopUp::POPUP_SIZE.height / 2),
                        Vec2(PopUp::POPUP_SIZE.width / 2, PopUp::POPUP_SIZE.height / 2), cocos2d::Color4F::GRAY);
    this->addChild(base);

    auto titleBase = DrawNode::create();
    titleBase->drawSolidRect(Vec2(-POPUP_SIZE.width / 2, POPUP_SIZE.height / 2 - 40.f),
                             Vec2(POPUP_SIZE.width / 2, POPUP_SIZE.height / 2), cocos2d::Color4F::GREEN);
    this->addChild(titleBase, 1);

    _title = cocos2d::Label::createWithTTF(title, Variables::FONT_NAME, Variables::FONT_SIZE);
    _title->setPosition(0.f, POPUP_SIZE.height / 2 - 20.f);
    this->addChild(_title, 2);

//    Sprite *closeNormal = Sprite::createWithSpriteFrameName(Variables::CLOSE_BUTTON_PATH);
//    Sprite *closeSelected = Sprite::createWithSpriteFrameName(Variables::CLOSE_BUTTON_PATH);
//    MenuItemSprite *close = MenuItemSprite::create(closeNormal, closeSelected, this,
//                                                   CC_CALLBACK_0(PopUp::closePopup, this));
//
//    _close = Menu::create(close, NULL);
//    _close->setPosition(POPUP_SIZE.width / 2 - 20.f, POPUP_SIZE.height / 2 - 20.f);
//    this->addChild(_close, 2);

    //   BlockPassingTouch();
}

PopUp::PopUp(std::string title, cocos2d::Node *message, CallFunc *okCallback) : PopUp(title) {
    message->setPosition(0, 0);
    this->addChild(message, 2);

//    Sprite *buttonNormal = Sprite::createWithSpriteFrameName(Variables::BUTTON_PATH);
//    Sprite *buttoSelected = Sprite::createWithSpriteFrameName(Variables::BUTTON_PATH);
//
//    MenuItemSprite *ok = MenuItemSprite::create(buttonNormal, buttoSelected, this, okCallback);
//
//    _buttons = Menu::create(ok, NULL);
//    _buttons->setPosition(0, -POPUP_SIZE.height / 2);
//    this->addChild(_buttons, 2);
}

PopUp::PopUp(std::string title, cocos2d::Node *message, CallFunc *yesCallback,
             CallFunc *noCallback) : PopUp(title) {
    message->setPosition(0, 0);
    this->addChild(message, 2);

//    MenuItemSprite *yes = MenuItemSprite::create(Sprite::createWithSpriteFrameName(Variables::YES_BUTTON_PATH),
//                                                 Sprite::createWithSpriteFrameName(Variables::YES_BUTTON_PATH), this,
//                                                 yesCallback);
//
//    MenuItemSprite *no;
//    if (noCallback == NULL) {
//        no = MenuItemSprite::create(Sprite::createWithSpriteFrameName(Variables::NO_BUTTON_PATH),
//                                    Sprite::createWithSpriteFrameName(Variables::NO_BUTTON_PATH), this,
//                                    CC_CALLBACK_0(PopUp::closePopup, this));
//    } else
//        no = MenuItemSprite::create(Sprite::createWithSpriteFrameName(Variables::NO_BUTTON_PATH),
//                                    Sprite::createWithSpriteFrameName(Variables::NO_BUTTON_PATH), this,
//                                    noCallback);
//
//

    MenuItem *yes = MenuItemFont::create("YES", CC_CALLBACK_0(PopUp::_popScene, this));

    MenuItemFont *no;
    if (noCallback == NULL) {
        no = MenuItemFont::create("NO", CC_CALLBACK_0(PopUp::closePopup, this));
    } else
        no = MenuItemFont::create("NO", CC_CALLBACK_0(PopUp::closePopup, this));


    _buttons = Menu::create(yes, no, NULL);
    _buttons->alignItemsHorizontallyWithPadding(30.f);
    _buttons->setPosition(0, -POPUP_SIZE.height / 2 + 30.f);
    this->addChild(_buttons, 2);
}

void PopUp::BlockPassingTouch() {
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch *touch, Event *event) {
        if (event->getCurrentTarget()->isVisible() == false)
            return false;
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void PopUp::closePopup() {
    BattleScene::instance->_unPause();
    this->removeFromParentAndCleanup(false);
}

void PopUp::_popScene() {
    BattleScene::instance->_onPopScene();
}

