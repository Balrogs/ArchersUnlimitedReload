#include "Randomizer.h"
#include "MainMenu.h"

bool Randomizer::init() {
    if (!Layer::init()) {
        return false;
    }

    _visibleSize = Director::getInstance()->getVisibleSize();


    _keyboardListener = cocos2d::EventListenerKeyboard::create();
    _keyboardListener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
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


    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    this->pause();

    return true;
}

void Randomizer::onEnter() {
    Node::onEnter();
    //this->resume();
}

void Randomizer::onQuit() {
    this->pause();
    MainScene::getInstance()->popAndReplace();
}
