#include <GameEngine/Global/Variables.h>
#include <Scenes/MenuLayers/Main/MainMenu.h>
#include "RewardButton.h"

USING_NS_CC;


RewardButton*
RewardButton::create(std::string itemType, int value, std::string buttonPath, std::string buttonPressedPath) {
    if(itemType == "GOLD"){
        RewardButtonCoin *ret = new(std::nothrow) RewardButtonCoin();
        if (ret && ret->init(value, buttonPath, buttonPressedPath)) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    } else if(itemType == "HAT"){
        RewardButtonHat *ret = new(std::nothrow) RewardButtonHat();
        if (ret && ret->init(value, buttonPath, buttonPressedPath)) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    } else if(itemType == "BOW"){
        RewardButtonBow *ret = new(std::nothrow) RewardButtonBow();
        if (ret && ret->init(value, buttonPath, buttonPressedPath)) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    } else if(itemType == "ARROW"){
        RewardButtonArrow *ret = new(std::nothrow) RewardButtonArrow();
        if (ret && ret->init(value, buttonPath, buttonPressedPath)) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    } else {
        return nullptr;
    }
}

bool RewardButton::init(std::string buttonPath, std::string buttonPressedPath) {
    if(!Node::init()){
        return  false;
    }

    _button = cocos2d::ui::Button::create();
    _button->loadTextures(buttonPath, buttonPressedPath, buttonPath, cocos2d::ui::Widget::TextureResType::PLIST);

    this->addChild(_button);

    return true;
}

void RewardButton::setEnabled(bool enabled) {
    _button->setEnabled(enabled);
}

const cocos2d::Size &RewardButton::getContentSize() {
    return _button->getContentSize();
}

bool RewardButtonCoin::init(int value, std::string buttonPath, std::string buttonPressedPath) {
    if(!RewardButton::init(buttonPath, buttonPressedPath)){
        return  false;
    }

    auto coinLabel = cocos2d::Label::createWithTTF(StringUtils::toString(value),
                                                   Variables::FONT_NAME,
                                                   Variables::FONT_SIZE_S());
    coinLabel->setColor(Color3B::BLACK);
    coinLabel->setPosition(_button->getContentSize().width / 2,
                           coinLabel->getContentSize().height);

    _button->addChild(coinLabel, 2);

    _button->addTouchEventListener([&, value](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                this->runAction(
                        CallFunc::create([value](){
                            cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();
                            auto coins = def->getIntegerForKey("COINS", 0);
                            def->setIntegerForKey("COINS", coins + value);
                            MainScene::getInstance()->popAndReplace();
                        }));
            }
                break;
            default:
                break;
        }
    });

    return true;
}

bool RewardButtonArrow::init(int value, std::string buttonPath, std::string buttonPressedPath) {
    if(!RewardButton::init(buttonPath, buttonPressedPath)){
        return  false;
    }

    _button->addTouchEventListener([&, value](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                this->runAction(
                        CallFunc::create([value](){
                            MainScene::getInstance()->popAndReplace();
                            if(EquipmentScene::getInstance()->resumeEquipment(1, value)){
                                MainScene::getInstance()->popMain();
                            }
                        }));
            }
                break;
            default:
                break;
        }
    });

    return true;
}

bool RewardButtonBow::init(int value, std::string buttonPath, std::string buttonPressedPath) {
    if(!RewardButton::init(buttonPath, buttonPressedPath)){
        return  false;
    }

    _button->addTouchEventListener([&, value](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                this->runAction(
                        CallFunc::create([value](){
                            MainScene::getInstance()->popAndReplace();
                            if(EquipmentScene::getInstance()->resumeEquipment(2, value)){
                                MainScene::getInstance()->popMain();
                            }
                        }));
            }
                break;
            default:
                break;
        }
    });


    return true;
}

bool RewardButtonHat::init(int value, std::string buttonPath, std::string buttonPressedPath) {
    if(!RewardButton::init(buttonPath, buttonPressedPath)){
        return  false;
    }

    _button->addTouchEventListener([&, value](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                this->runAction(
                        CallFunc::create([value](){
                            MainScene::getInstance()->popAndReplace();
                            if(EquipmentScene::getInstance()->resumeEquipment(0, value)){
                                MainScene::getInstance()->popMain();
                            }
                        }));
            }
                break;
            default:
                break;
        }
    });

    return true;
}

bool RewardButtonDisabled::init() {
    return RewardButton::init(Variables::GIFT_DISABLED, Variables::GIFT_DISABLED);
}
