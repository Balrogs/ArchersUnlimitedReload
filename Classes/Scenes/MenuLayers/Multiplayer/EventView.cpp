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

    _def = cocos2d::UserDefault::getInstance();

    _visibleSize = Director::getInstance()->getVisibleSize();

    _bg = Sprite::createWithSpriteFrameName(Variables::BG1);

    _bg->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);

    _bg->setScale((_visibleSize.width - 50.f) / _bg->getContentSize().width,
                  (_visibleSize.height - 50.f) / _bg->getContentSize().height);

    this->addChild(_bg, 1);

    auto title = cocos2d::Label::createWithTTF( LocalizedStrings::getInstance()->getString("EVENTS"), Variables::FONT_NAME,
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

    this->setPosition(_visibleSize.width, 0);

    // TODO show event table(label)
    // TODO current reward

    auto timeButton = InfoButtonTime::create();
    timeButton->setPosition(_visibleSize.width / 2, _visibleSize.height / 3);
    this->addChild(timeButton, 3);
    
    Clocks* eventEndClock = Clocks::create(Variables::getCurrentTime() + 3 * 60 * 60 * 1000, [&](){
        _updateEvent();
    });
    eventEndClock->setPosition(Vec2(
            _bg->getBoundingBox().getMaxX() - 50.f - title->getContentSize().width / 2,
            title->getPosition().y
    ));

    this->addChild(eventEndClock, 2);


    auto timeLeft = cocos2d::Label::createWithTTF( LocalizedStrings::getInstance()->getString("TIMELEFT"), Variables::FONT_NAME,
                                                   Variables::FONT_SIZE());
    timeLeft->setPosition(eventEndClock->getPosition().x - timeLeft->getContentSize().width - 50.f,
                          eventEndClock->getPosition().y);
    this->addChild(timeLeft, 2);

    _richEventButton = RichEventButton::create();

    _richEventButton->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);

    this->addChild(_richEventButton, 3);
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

void EventView::_updateEvent() {

}
