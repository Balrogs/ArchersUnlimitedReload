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

    this->setPosition(-_visibleSize.width, 0);

    return true;
}

void Randomizer::onEnter() {
    Node::onEnter();
    this->runAction(Sequence::create(
            CallFunc::create([](){
                MainScene::getInstance()->wait(true);
            }),
            MoveTo::create(0.5f, Vec2::ZERO)
            , NULL)
    );
}

void Randomizer::onQuit() {
    this->runAction(Sequence::create(
            CallFunc::create([](){
                MainScene::getInstance()->wait(false);
            }),
            MoveTo::create(0.5f, Vec2(-_visibleSize.width, 0)),
            CallFunc::create([](){
                MainScene::getInstance()->popAndReplace();
            }), NULL)
    );
}
