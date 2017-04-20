#include "EventView.h"
#include <GameEngine/Global/Variables.h>
#include <Localization/LocalizedStrings.h>
#include <GameEngine/Global/Misc/UI/Clocks.h>
#include <GameEngine/Global/Misc/UI/RichEventButton.h>
#include <GameEngine/Global/Misc/UI/InfoButton.h>
#include "Scenes/MenuLayers/Main/MainMenu.h"
#include "Lobby.h"

USING_NS_CC;

bool EventView::init() {
    if (!Layer::init()) {
        return false;
    }

    _client =SocketClient::getInstance();

    _def = cocos2d::UserDefault::getInstance();

    _visibleSize = Director::getInstance()->getVisibleSize();

    _bg = Sprite::createWithSpriteFrameName(Variables::BG1);

    _bg->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);

    _bg->setScale((_visibleSize.width - 50.f) / _bg->getContentSize().width,
                  (_visibleSize.height - 50.f) / _bg->getContentSize().height);

    this->addChild(_bg, 1);

    _title = cocos2d::Label::createWithTTF( LocalizedStrings::getInstance()->getString("EVENTS"), Variables::FONT_NAME,
                                                Variables::H_FONT_SIZE());
    _title->setPosition(_bg->getBoundingBox().getMinX() + 50.f + _title->getContentSize().width / 2,
                       _bg->getBoundingBox().getMaxY() - _title->getContentSize().height / 2 - 50.f);
    this->addChild(_title, 2);

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

    this->setPosition(_visibleSize.width, 0);

    auto timeLeft = cocos2d::Label::createWithTTF( LocalizedStrings::getInstance()->getString("TIMELEFT"), Variables::FONT_NAME,
                                                   Variables::FONT_SIZE());
    timeLeft->setPosition(_bg->getBoundingBox().getMaxX() - 50.f - _title->getContentSize().width / 2 - timeLeft->getContentSize().width - 50.f,
                          _title->getPosition().y);
    this->addChild(timeLeft, 2);

    _richEventButton = RichEventButton::create();

    _richEventButton->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);

    this->addChild(_richEventButton, 3);

    _client->getEventInfo();

    return true;
}

void EventView::onEnter() {
    Layer::onEnter();
    this->runAction(MoveTo::create(0.5f, Vec2::ZERO));
}

void EventView::onQuit() {
    _richEventButton->saveTime();
    this->runAction(Sequence::create(
            MoveTo::create(0.5f, Vec2(_visibleSize.width, 0)),
            CallFunc::create([](){
                MainScene::getInstance()->replaceMain(Lobby::create());
            }), NULL)
    );
}

void EventView::updateEvent(EventInfo* info) {
    if(_info->getId() == info->getId()) {
        return;
    }

    _info = info;

    if(_clocks){
        _clocks->removeFromParent();
    }
    _clocks = Clocks::create(info->getEndTime() * 1000, [&](){
        _updateEvent();
    });
    _clocks->setPosition(Vec2(
            _bg->getBoundingBox().getMaxX() - 50.f - _title->getContentSize().width / 2,
            _title->getPosition().y
    ));
    this->addChild(_clocks, 2);

    //TODO update info

    _client->getPlayerInfo(_info->getId(), _client->getDBPlayer()->getName());
}

void EventView::_updateEvent() {
    _client->getEventInfo();
}
