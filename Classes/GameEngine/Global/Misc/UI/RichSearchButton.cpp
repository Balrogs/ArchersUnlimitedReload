#include <GameEngine/Global/Variables.h>
#include <Scenes/MenuLayers/Main/MainMenu.h>
#include <Scenes/MenuLayers/Multiplayer/Lobby.h>
#include "RichSearchButton.h"

bool RichSearchButton::init() {
    if(!Node::init()){
        return  false;
    }

    auto button = ui::Button::create(Variables::FIND_BUTTON, Variables::FIND_BUTTON, Variables::FIND_BUTTON, ui::Widget::TextureResType::PLIST);

    button->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                switch (_state) {
                    case State::Busy : {
                        break;
                    }
                    case State::Searching : {
                        this->reset();
                        break;
                    }

                    case State::Idle : {
                        this->start();
                        break;
                    }
                }
                default:
                    break;
            }
        }
    });

    this->addChild(button);

    _lobby = MainScene::getInstance()->getMain();

    _stopButton = Sprite::createWithSpriteFrameName(Variables::CLOSE_BUTTON_PATH);

    _scale = 0.75f * button->getBoundingBox().size.width / _stopButton->getBoundingBox().size.width;
    //_stopButton->setPosition(button->getBoundingBox().size.width / 2, button->getBoundingBox().size.height / 2);
    _stopButton->setScale(0);

    this->addChild(_stopButton);

    reset();

    return true;
}

void RichSearchButton::start() {
    _switchState(State::Searching);

}

void RichSearchButton::reset() {
    _switchState(State::Idle);
}

void RichSearchButton::_switchState(State state) {
    if(state == _state){
        return;
    }

    switch(state){
        case State::Busy : {
            _state = state;
            break;
        }

        case State::Searching : {
            _stopButton->runAction(RepeatForever::create(
                    Sequence::create(
                            DelayTime::create(.5f),
                            RotateBy::create(1.f, 360.f),
                            DelayTime::create(.3f),
                            RotateBy::create(1.f, -360.f),
                            NULL
                    )));

            _stopButton->runAction(
                    Sequence::create(
                            Spawn::create(
                                    CallFunc::create(
                                            [&]() {
                                                _switchState(State::Busy);
                                            }
                                    ),
                                    ScaleTo::create(.5f, _scale),
                                    RotateBy::create(.5f, 360.f),
                            NULL),
                            CallFunc::create(
                                    [&]() {
                                        _state = State::Searching;
                                        if(auto lobby = dynamic_cast<Lobby*>(MainScene::getInstance()->getMain())){
                                            lobby->showSearchPopUp();
                                        }
                                    }
                            ),
                            NULL
                    ));

            break;
        }

        case State::Idle : {
            _stopButton->runAction(
                    Spawn::create(
                            CallFunc::create(
                                    [&]() {
                                        _switchState(State::Busy);
                                    }
                            ),
                            Sequence::create(
                                    ScaleTo::create(.5f, 0.f),
                                    CallFunc::create(
                                            [&]() {
                                                _stopButton->getActionManager()->removeAllActions();
                                                _state = State::Idle;
                                                if(auto lobby = dynamic_cast<Lobby*>(MainScene::getInstance()->getMain())){
                                                    lobby->leaveLobby();
                                                }
                                            }
                                    ),
                                    NULL
                            ),
                            NULL
                    )
            );
            break;
        }
    }
}