#include "EventView.h"
#include <Scenes/Layers/BackgroundLayer.h>
#include <ui/UIButton.h>
#include <GameEngine/Global/Variables.h>
#include <Localization/LocalizedStrings.h>
#include "Scenes/MenuLayers/Main/MainMenu.h"
#include "Lobby.h"

USING_NS_CC;

bool EventView::init() {
    if (!Layer::init()) {
        return false;
    }

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

    return true;
}

void EventView::onEnter() {
    Layer::onEnter();
    this->runAction(Sequence::create(
            CallFunc::create([](){
                MainScene::getInstance()->wait(true);
            }),
            MoveTo::create(0.5f, Vec2::ZERO)
            , NULL)
    );
}

void EventView::onQuit() {
    this->runAction(Sequence::create(
            MoveTo::create(0.5f, Vec2(_visibleSize.width, 0)),
            CallFunc::create([](){
                MainScene::getInstance()->replaceMain(Lobby::create());
            }), NULL)
    );
}
