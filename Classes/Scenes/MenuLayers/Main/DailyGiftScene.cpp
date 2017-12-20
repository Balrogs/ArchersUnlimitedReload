#include <GameEngine/Global/Variables.h>
#include <Localization/LocalizedStrings.h>
#include <GameEngine/Global/Misc/JSONParser.h>
#include "DailyGiftScene.h"
#include "MainMenu.h"

USING_NS_CC;

const long DailyGiftScene::DAY_IN_MILLIS = 86400000l;

bool DailyGiftScene::init() {
    if (!Layer::init()) {
        return false;
    }

    _def = cocos2d::UserDefault::getInstance();

    _visibleSize = Director::getInstance()->getVisibleSize();

    _bg = Sprite::createWithSpriteFrameName(Variables::BG1);

    _bg->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);

    _bg->setScale((2 * _visibleSize.width / 3) / _bg->getContentSize().width,
                  (_visibleSize.height - 50.f) / _bg->getContentSize().height);

    this->addChild(_bg, 1);

    auto title = cocos2d::Label::createWithTTF( LocalizedStrings::getInstance()->getString("GET YOUR DAILY GIFT"), Variables::FONT_NAME,
                                                Variables::H_FONT_SIZE());
    title->setPosition(_bg->getPositionX(),
                       _bg->getBoundingBox().getMaxY() - .8f * title->getContentSize().height);
    this->addChild(title, 2);

    auto day = _getDay();
    auto dayLabel = cocos2d::Label::createWithTTF(StringUtils::format("%d %s", day, LocalizedStrings::getInstance()->getString("DAY")), Variables::FONT_NAME,
                                                Variables::H_FONT_SIZE());
    dayLabel->setPosition(_bg->getPositionX(),
                          title->getPositionX() - dayLabel->getContentSize().height);
    this->addChild(dayLabel, 2);

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

    Size size = Size(_visibleSize.width / 12, _visibleSize.width / 12);

    _generateList(day, size);

    return true;
}

void DailyGiftScene::_generateList(int day, Size size) {
    int factor = day / 9;
    switch(factor) {
        case 0: {
            int array[] = {0, 1, 5, 0, 1, 3, 1, 1, 6};
            std::vector<int> items(array, array + 9);
            _setItems(items, day, size);
            break;
        }
        case 1: {
            int array[] = {0, 1, 4, 0, 1, 2, 1, 1, 7};
            std::vector<int> items(array, array + 9);
            _setItems(items, day, size);
            break;
        }
        default: {
            int array[] = {0, 1, 2, 0, 1, 2, 0, 1, 2};
            std::vector<int> items(array, array + 9);
            _setItems(items, day, size);
            break;
        }
    }
}

void DailyGiftScene::_setItems(std::vector<int> items, int day, Size size) {
    int decade = (day / 10) * 9;
    int id = day % 9;
    int columnCount = 3;
    int rowCount = 3;
    float startPosX =  _bg->getPosition().x - 1.5f * 1.2f * size.width;
    for(auto i = 0; i < items.size() ;i++) {
        bool enabled = false;
        if(i + 1 == id) {
            enabled = true;
        }

        int itemId = items[i];

        if(i + 1 < id) {
            itemId = -1;
        }

        int row = rowCount - i / rowCount;
        int column = i % columnCount;

        Vec2 pos = Vec2(startPosX + 1.2f * size.width * column + 2 * Variables::FONT_SIZE_S(), _bg->getBoundingBox().getMinY() + 1.2f * size.width * row);

        auto dayLabel = cocos2d::Label::createWithTTF(StringUtils::format("%d %s", decade + i + 1, LocalizedStrings::getInstance()->getString("DAY")), Variables::FONT_NAME,
                                                      Variables::FONT_SIZE_S());
        dayLabel->setPosition(
                pos.x,
                pos.y + size.height / 2 + dayLabel->getContentSize().height / 2
        );

        this->addChild(dayLabel, 2);

        _addItem(itemId, pos, size, enabled);
    }
}

void DailyGiftScene::_addItem(int id, cocos2d::Vec2 position, Size size, bool enabled) {
    auto item = JSONParser::parseLocalRewards(id);

    item->setScale(size.width / item->getContentSize().width, size.height / item->getContentSize().height);

    if(enabled) {
        auto hightlight = Sprite::createWithSpriteFrameName(Variables::GIFT_HIGHTLIGHT);
        hightlight->setScale(1.1f * size.width / hightlight->getContentSize().width, 1.1f * size.height / hightlight->getContentSize().height);
        hightlight->setPosition(position);
        hightlight->runAction(RepeatForever::create(Sequence::create(
                ToggleVisibility::create(),
                DelayTime::create(.8f),
                Hide::create(),
                DelayTime::create(.8f),
                NULL
        )));
        this->addChild(hightlight, 2);
    }

    item->setEnabled(enabled);
    item->setPosition(position);

    this->addChild(item, 3);
}

void DailyGiftScene::onEnter() {
    Layer::onEnter();
    this->runAction(Sequence::create(
            CallFunc::create([](){
                MainScene::getInstance()->wait(true);
            }),
            MoveTo::create(0.5f, Vec2::ZERO)
            , NULL)
    );
}

void DailyGiftScene::onQuit() {
    this->runAction(Sequence::create(
            CallFunc::create([](){
                MainScene::getInstance()->wait(false);
            }),
            MoveTo::create(0.5f, Vec2(_visibleSize.width, 0)),
            CallFunc::create([](){
                MainScene::getInstance()->popAndReplace();
            }), NULL)
    );
}

bool DailyGiftScene::checkDate() {
    auto timeStr = cocos2d::UserDefault::getInstance()->getStringForKey("DailyGiftLastDate", "0");
    if(timeStr != "0") {
        auto ts = std::atol(timeStr.c_str());
        auto curr = Variables::getCurrentTime();
        auto diff = curr - ts;
        return diff >= DailyGiftScene::DAY_IN_MILLIS;
    }
    return true;
}

int DailyGiftScene::_getDay() {
    return cocos2d::UserDefault::getInstance()->getIntegerForKey("DailyGiftDaysCounter", 1);
}


