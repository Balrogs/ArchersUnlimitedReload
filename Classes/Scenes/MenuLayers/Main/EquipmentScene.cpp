#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/JSONParser.h>
#include <Localization/LocalizedStrings.h>
#include "EquipmentScene.h"
#include "MainMenu.h"

USING_NS_CC;

EquipmentScene *EquipmentScene::_instance = nullptr;

bool EquipmentScene::init() {
    if (!Layer::init()) {
        return false;
    }

    cocos2d::Director::getInstance()->getScheduler()->schedule(
            schedule_selector(EquipmentScene::_enterFrameHandler),
            this, 0.f, false
    );

    _busy = false;
    _paused = true;
    _instance = this;

    factory.loadDragonBonesData("ArcUnlimArmature.json");
    factory.loadTextureAtlasData("texture.json");

    _visibleSize = Director::getInstance()->getVisibleSize();


    _keyboardListener = cocos2d::EventListenerKeyboard::create();
    _keyboardListener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
        switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_BREAK:
            case EventKeyboard::KeyCode::KEY_ESCAPE:
            case EventKeyboard::KeyCode::KEY_BACKSPACE: {
                if(!_paused){
                    onQuit();
                }
            }
                break;
            default:
                break;
        }
    };

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);


    auto color = Color3B(51, 51, 51);

    _hatsCount = Label::createWithTTF("", Variables::FONT_NAME, Variables::FONT_SIZE());
    _hatsCount->setPosition(Vec2(
            _visibleSize.width * 0.8f,
            _visibleSize.height * 0.68f)
    );
    _hatsCount->setColor(color);
    _hatsCount->setVisible(false);
    this->addChild(_hatsCount);
    _arrowsCount = Label::createWithTTF("", Variables::FONT_NAME, Variables::FONT_SIZE());
    _arrowsCount->setPosition(Vec2(
            _hatsCount->getPositionX(),
            _hatsCount->getPositionY() - 1.5f * Variables::FONT_SIZE())
    );
    _arrowsCount->setColor(color);
    _arrowsCount->setVisible(false);
    this->addChild(_arrowsCount);
    _bowsCount = Label::createWithTTF("", Variables::FONT_NAME, Variables::FONT_SIZE());
    _bowsCount->setPosition(Vec2(
            _hatsCount->getPositionX(),
            _arrowsCount->getPositionY() - 1.5f * Variables::FONT_SIZE())
    );
    _bowsCount->setColor(color);
    _bowsCount->setVisible(false);
    this->addChild(_bowsCount);

    _readAssets();

    _hero = new HeroPreview();
    _hero->setScale(1.5f);
    _hero->setPosition(_visibleSize.width / 4, _visibleSize.height / 10);

    this->addChild(_hero, 2);

    _heroBox = Rect(_hero->getPosition().x * 0.5f, _hero->getPosition().y, _hero->getPosition().x,  _hero->getGlobalHeight("head"));

    this->pause();

    return true;

}

bool EquipmentScene::resumeEquipment() {
    if(_busy){
        return false;
    }

    _paused = false;
    _busy = true;

    Layer::resume();

    _readAssets();

    this->runAction(Sequence::create(
//            Spawn::createWithTwoActions(
//                    ScaleTo::create(.8f, 1.3f),
//                    MoveTo::create(.8f, Vec2(
//                            _visibleSize.width / 2 - _stand->getContentSize().width / 2,
//                            _stand->getContentSize().height / 2)
//                    )
//            ),
            CallFunc::create([&]() {
                _controls = UIControls::create(_hero);
                _controls->setPosition(Vec2(0, 0));
                _hero->addChild(_controls, 2);

                _bowsCount->setVisible(true);
                _hatsCount->setVisible(true);
                _arrowsCount->setVisible(true);
                _busy = false;
            }),
            NULL));
    return true;
}

void EquipmentScene::onQuit() {
    if(_busy){
        return;
    }

    _paused = true;
    _busy = true;

    this->pause();

    _hero->runAction(Sequence::create(
            CallFunc::create([&]() {
                if(_controls != nullptr) {
                    _controls->removeFromParent();
                    _bowsCount->setVisible(false);
                    _hatsCount->setVisible(false);
                    _arrowsCount->setVisible(false);
                    _hero->getShoulders()->getAnimation().fadeIn("equipment_idle", -1.f, 1);
                }
            }),
//            Spawn::createWithTwoActions(
//                    ScaleTo::create(.7f, 1.f),
//                    MoveTo::create(.7f, Vec2(_stand->getContentSize().width / 2, _stand->getContentSize().height / 2))
//            ),
            CallFunc::create([&]() {
                _busy = false;
            }),
            NULL));
    MainScene::getInstance()->popAndReplace();
}

cocos2d::Vec2 EquipmentScene::getButtonPosition() {
    return Vec2(_hero->getPosition().x, _hero->getGlobalHeight("head") * 0.8f);
}

void EquipmentScene::_readAssets() {
    auto arrows = JSONParser::parseAsset("arrows");
    auto bows = JSONParser::parseAsset("bows");
    auto hats = JSONParser::parseAsset("hats");

    int arrowsCount = 0;
    int bowsCount = 0;
    int hatsCount = 0;

    for (auto arrow : arrows) {
        if (arrow->isAvailable()) {
            arrowsCount++;
        }
    }

    for (auto bow : bows) {
        if (bow->isAvailable()) {
            bowsCount++;
        }
    }

    for (auto hat : hats) {
        if (hat->isAvailable()) {
            hatsCount++;
        }
    }

    _hatsCount->setString(StringUtils::format("%s %d / %d",
                                              LocalizedStrings::getInstance()->getString("HATS"),
                                              hatsCount, (int) hats.size()));
    _bowsCount->setString(StringUtils::format(" %s %d / %d",
                                              LocalizedStrings::getInstance()->getString("BOWS"),
                                              bowsCount, (int) bows.size()));
    _arrowsCount->setString(StringUtils::format("%s %d / %d",
                                                LocalizedStrings::getInstance()->getString("ARROWS"),
                                                arrowsCount, (int) arrows.size()));
}

EquipmentScene *EquipmentScene::getInstance() {
    return _instance;
}

void EquipmentScene::_enterFrameHandler(float passedTime) {
    dragonBones::WorldClock::clock.advanceTime(passedTime);
}

bool EquipmentScene::checkTouch(Vec2 pos) {
    return _heroBox.containsPoint(pos);
}