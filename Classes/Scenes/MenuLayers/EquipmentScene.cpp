#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/JSONParser.h>
#include <Localization/LocalizedStrings.h>
#include "EquipmentScene.h"
#include "MainMenu.h"

USING_NS_CC;

bool EquipmentScene::init() {
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

    _stand = Sprite::createWithSpriteFrameName(Variables::STAND);
    _stand->setPosition(Vec2(_stand->getContentSize().width / 2, _stand->getContentSize().height / 2));
    this->addChild(_stand, 1);

    auto color = Color3B(51,51,51);

    _hatsCount = Label::createWithTTF("", Variables::FONT_NAME, Variables::FONT_SIZE);
    _hatsCount->setPosition(Vec2(
            _visibleSize.width * 0.8f,
            _visibleSize.height * 0.68f)
    );
    _hatsCount->setColor(color);
    _hatsCount->setVisible(false);
    this->addChild(_hatsCount);
    _arrowsCount = Label::createWithTTF("", Variables::FONT_NAME, Variables::FONT_SIZE);
    _arrowsCount->setPosition(Vec2(
            _hatsCount->getPositionX(),
            _hatsCount->getPositionY() - 1.5f *Variables::FONT_SIZE)
    );
    _arrowsCount->setColor(color);
    _arrowsCount->setVisible(false);
    this->addChild(_arrowsCount);
    _bowsCount = Label::createWithTTF("", Variables::FONT_NAME, Variables::FONT_SIZE);
    _bowsCount->setPosition(Vec2(
            _hatsCount->getPositionX(),
            _arrowsCount->getPositionY() - 1.5f *Variables::FONT_SIZE)
    );
    _bowsCount->setColor(color);
    _bowsCount->setVisible(false);
    this->addChild(_bowsCount);
    
    _readAssets();

    auto hero = new HeroPreview();
    hero->setScale(1.5f);
    hero->setPosition(_stand->getContentSize().width / 2, _stand->getContentSize().height / 2);
    _stand->addChild(hero, 2);

    _controls = UIControls::create(hero);
    _controls->setPosition(Vec2(
            0,
            0)
    );
    hero->addChild(_controls, 2);

    this->pause();
    return true;
}

void EquipmentScene::resumeEquipment() {
    Layer::resume();

    _readAssets();

    _stand->runAction(Sequence::createWithTwoActions(
            Spawn::createWithTwoActions(
                    ScaleTo::create(.8f, 1.3f),
                    MoveTo::create(.8f, Vec2(
                            _visibleSize.width / 2 - _stand->getContentSize().width / 2,
                            _stand->getContentSize().height / 2)
                    )
            ),
            CallFunc::create([&](){
                _controls->setVisible(true);
                _bowsCount->setVisible(true);
                _hatsCount->setVisible(true);
                _arrowsCount->setVisible(true);
            })
                     )
    );

}

void EquipmentScene::onQuit() {

    this->pause();

    _stand->runAction(Sequence::createWithTwoActions(
            CallFunc::create([&](){
                _controls->setVisible(false);
                _bowsCount->setVisible(false);
                _hatsCount->setVisible(false);
                _arrowsCount->setVisible(false);
            }),
            Spawn::createWithTwoActions(
                    ScaleTo::create(.7f, 1.f),
                    MoveTo::create(.7f, Vec2(_stand->getContentSize().width / 2, _stand->getContentSize().height / 2))
            )
                     )
    );
    MainScene::getInstance()->popAndReplace();
}

cocos2d::Vec2 EquipmentScene::getButtonPosition() {
    return Vec2(_controls->getHero()->getPosition().x, _controls->getHero()->getGlobalHeight("head"));
}

void EquipmentScene::_readAssets() {
    auto arrows = JSONParser::parseAsset("arrows");
    auto bows = JSONParser::parseAsset("bows");
    auto hats = JSONParser::parseAsset("hats");

    int arrowsCount = 0;
    int bowsCount = 0;
    int hatsCount = 0;

    for(auto arrow : arrows){
        if(arrow->isAvailable()){
            arrowsCount++;
        }
    }

    for(auto bow : bows){
        if(bow->isAvailable()){
            bowsCount++;
        }
    }

    for(auto hat : hats){
        if(hat->isAvailable()){
            hatsCount++;
        }
    }

    _hatsCount->setString(StringUtils::format("%s %d / %d",
                                              LocalizedStrings::getInstance()->getString("HATS"),
                                              hatsCount, (int)hats.size()));
    _bowsCount->setString(StringUtils::format(" %s %d / %d",
                                              LocalizedStrings::getInstance()->getString("BOWS"),
                                              bowsCount, (int)bows.size()));
    _arrowsCount->setString(StringUtils::format("%s %d / %d",
                                                LocalizedStrings::getInstance()->getString("ARROWS"),
                                                arrowsCount, (int)arrows.size()));
}

EquipmentScene::UIControls *EquipmentScene::UIControls::create(HeroPreview *hero) {
    UIControls *ret = new(std::nothrow) UIControls();
    if (ret && ret->init(hero)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentScene::UIControls::init(HeroPreview *hero) {
    if (!Node::init()){
        return false;
    }

    _hero = hero;

    auto heroHeight = _hero->getHatHeight();

    auto width = Director::getInstance()->getVisibleSize().width * 0.1f;

    auto arrowSelectorArea = Rect(Vec2(-width, 0), Size(width, heroHeight));
    auto hatSelectorArea = Rect(Vec2(- width , heroHeight), Size(2 * width, heroHeight * 0.7f));
    auto bowSelectorArea = Rect(Vec2::ZERO, Size(width, heroHeight));
    
    auto arrowSelectorAreaNode = DrawNode::create();
    arrowSelectorAreaNode->drawRect(Vec2(arrowSelectorArea.getMinX(),arrowSelectorArea.getMinY()), 
                                    Vec2(arrowSelectorArea.getMaxX(),arrowSelectorArea.getMaxY()),
                                         Color4F::BLUE);
    this->addChild(arrowSelectorAreaNode);

    auto hatSelectorAreaNode = DrawNode::create();
    hatSelectorAreaNode->drawRect(Vec2(hatSelectorArea.getMinX(),hatSelectorArea.getMinY()),
                                    Vec2(hatSelectorArea.getMaxX(),hatSelectorArea.getMaxY()),
                                    Color4F::RED);
    this->addChild(hatSelectorAreaNode);

    auto bowSelectorAreaNode = DrawNode::create();
    bowSelectorAreaNode->drawRect(Vec2(bowSelectorArea.getMinX(),bowSelectorArea.getMinY()),
                                    Vec2(bowSelectorArea.getMaxX(),bowSelectorArea.getMaxY()),
                                    Color4F::YELLOW);
    this->addChild(bowSelectorAreaNode);

    //TODO create selectors

    //TODO add touchListener

    //TODO pause touchListener when controls is hidden
    
    return true;
}

HeroPreview *EquipmentScene::UIControls::getHero() {
    return _hero;
}

EquipmentScene::Selector *EquipmentScene::Selector::create(EquipmentScene::Alignment alignment) {
    Selector *ret = new(std::nothrow) Selector();
    if (ret && ret->init(alignment)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentScene::Selector::init(EquipmentScene::Alignment alignment) {
    if (!Node::init()){
        return false;
    }

    switch(alignment){
        case Alignment::Horizontal: {
            break;
        }
        case Alignment::Vertical: {
            break;
        }
    }

    return true;
}

EquipmentScene::Item *EquipmentScene::Item::create(Node* view, int ind, bool isAvailable = true) {
    Item *ret = new(std::nothrow) Item();
    if (ret && ret->init(view, ind, isAvailable)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentScene::Item::init(Node* view, int ind, bool isAvailable) {
    if (!Sprite::init()){
        return false;
    }

    return true;
}
