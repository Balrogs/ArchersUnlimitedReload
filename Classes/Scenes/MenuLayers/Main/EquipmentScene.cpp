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
    _hero->setPosition(_stand->getContentSize().width / 2, _stand->getContentSize().height / 2);
    _stand->addChild(_hero, 2);

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
            CallFunc::create([&]() {
                _controls = UIControls::create(_hero);
                _controls->setPosition(Vec2(0, 0));
                _hero->addChild(_controls, 2);

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
            CallFunc::create([&]() {
                if(_controls) {
                    _controls->removeFromParent();
                    _bowsCount->setVisible(false);
                    _hatsCount->setVisible(false);
                    _arrowsCount->setVisible(false);
                    _hero->getShoulders()->getAnimation().fadeIn("equipment_idle", -1.f, 1);
                }
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
    return Vec2(_hero->getPosition().x, _hero->getGlobalHeight("head"));
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

std::string EquipmentScene::boneName(EquipmentScene::Type v) {
    switch (v) {
        case Arrow:
            return "arrow";
        case Bow:
            return "bow";
        case Hat:
            return "head";
    }
}

std::string EquipmentScene::animationName(EquipmentScene::Type v) {
    switch (v) {
        case Arrow:
            return "equipment_arrow";
        case Bow:
            return "equipment_bow";
        case Hat:
            return "equipment_idle";
    }
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
    if (!Node::init()) {
        return false;
    }
    _touch = -1;
    _hero = hero;

    auto heroHeight = _hero->getHatHeight();

    auto width = Director::getInstance()->getVisibleSize().width * 0.1f;


    auto arrowSelectorArea = Rect(Vec2(-width, 0), Size(width, heroHeight));
    auto hatSelectorArea = Rect(Vec2(-width, heroHeight), Size(2 * width, heroHeight * 0.7f));
    auto bowSelectorArea = Rect(Vec2::ZERO, Size(width, heroHeight));

//    auto arrowSelectorAreaNode = DrawNode::create();
//    arrowSelectorAreaNode->drawRect(Vec2(arrowSelectorArea.getMinX(), arrowSelectorArea.getMinY()),
//                                    Vec2(arrowSelectorArea.getMaxX(), arrowSelectorArea.getMaxY()),
//                                    Color4F::BLUE);
//    this->addChild(arrowSelectorAreaNode);
//
//    auto hatSelectorAreaNode = DrawNode::create();
//    hatSelectorAreaNode->drawRect(Vec2(hatSelectorArea.getMinX(), hatSelectorArea.getMinY()),
//                                  Vec2(hatSelectorArea.getMaxX(), hatSelectorArea.getMaxY()),
//                                  Color4F::RED);
//    this->addChild(hatSelectorAreaNode);
//
//    auto bowSelectorAreaNode = DrawNode::create();
//    bowSelectorAreaNode->drawRect(Vec2(bowSelectorArea.getMinX(), bowSelectorArea.getMinY()),
//                                  Vec2(bowSelectorArea.getMaxX(), bowSelectorArea.getMaxY()),
//                                  Color4F::YELLOW);
//    this->addChild(bowSelectorAreaNode);


    auto arrowTransform = _hero->getSlotPosition("arrow");
    auto arrowPoint = Vec3(arrowTransform.x, arrowTransform.y, 0);
    _hero->getWorldToNodeTransform().translate(arrowPoint);
    auto arrowCenter = Vec2(arrowPoint.x, arrowPoint.y);

    auto arrowSelector = SelectorVertical::create(arrowSelectorArea,
                                                  Vec2(100, 100),
                                                  -180,
                                                  _hero->getPlayerView()->getArrow()->Id(),
                                                  Type::Arrow);
    _selectors.push_back(arrowSelector);
    this->addChild(arrowSelector);


    auto bowTransform = _hero->getSlotPosition("bow");
    auto bowPoint = Vec3(bowTransform.x, bowTransform.y, 0);
    _hero->getWorldToNodeTransform().translate(bowPoint);
    auto bowCenter = Vec2(bowPoint.x, bowPoint.y);
    auto bowSelector = SelectorVertical::create(bowSelectorArea,
                                                bowCenter,
                                                0,
                                                _hero->getPlayerView()->getBow()->Id(),
                                                Type::Bow);
    _selectors.push_back(bowSelector);
    this->addChild(bowSelector);

    auto hatTransform = _hero->getSlotPosition("head");
    auto hatPoint = Vec3(hatTransform.x, -hatTransform.y, 0);
    _hero->getWorldToNodeTransform().translate(hatPoint);
    auto hatCenter = Vec2(hatPoint.x, hatPoint.y);

    auto hatSelector = SelectorHorizontal::create(hatSelectorArea,
                                                  hatCenter,
                                                  0,
                                                  _hero->getPlayerView()->getHat()->Id(),
                                                  Type::Hat);
    _selectors.push_back(hatSelector);
    this->addChild(hatSelector);

    const auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(UIControls::_touchHandlerBegin, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(UIControls::_touchHandlerMove, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(UIControls::_touchHandlerEnd, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    return true;
}

HeroPreview *EquipmentScene::UIControls::getHero() {
    return _hero;
}

bool EquipmentScene::UIControls::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_touch < 0) {
        _touch = touch->getID();
    } else {
        return false;
    }

    auto focused = _getFocused(touch->getStartLocation());

    if (focused == nullptr) {
        if (_focused != nullptr) {
            _focused->onFocused(false);
            _focused = nullptr;
        }
        return true;
    }

    if (_focused != focused) {
        if (_focused != nullptr) {
            _focused->onFocused(false);
        }
        _focused = focused;
        _focused->onFocused(true);
        return true;
    }

    return true;
}

bool EquipmentScene::UIControls::_touchHandlerMove(const cocos2d::Touch *touch, cocos2d::Event *event) {
    return _touch == touch->getID();

}

bool EquipmentScene::UIControls::_touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_touch == touch->getID()) {
        _touch = -1;
    } else {
        return false;
    }
    if (_focused == nullptr) {
        return true;
    }

    const auto start = touch->getStartLocation();
    const auto curr = touch->getLocation();
    int x = start.x - curr.x;
    int y = start.y - curr.y;
    _focused->scroll(x, y);

    return true;
}

EquipmentScene::Selector *EquipmentScene::UIControls::_getFocused(Vec2 pos) {
    Vec3 *point = new Vec3(pos.x, pos.y, 0);
    _hero->getWorldToNodeTransform().transformPoint(point);
    pos = Vec2(point->x, point->y);
    for (auto selector : _selectors) {
        if (selector->checkTouch(pos)) {
            return selector;
        }
    }
    return nullptr;
}

bool EquipmentScene::Selector::init(Rect rect, Vec2 center, float rotation, int index, Type type) {
    if (!Node::init()) {
        return false;
    }

    _action = Action::Idle;
    _isFocused = false;
    _type = type;
    _rect = rect;
    _rectFocused = rect;
    _index = index;

    std::string asset;

    switch (_type) {
        case Type::Arrow: {
            asset = "arrows";
            break;
        }
        case Type::Bow: {
            asset = "bows";
            break;
        }
        case Type::Hat: {
            asset = "hats";
            break;
        }
    }

    auto assets = JSONParser::parseAsset(asset);
    for (auto a : assets) {
        auto node = EquipmentScene::getInstance()->factory.buildArmatureDisplay(a->Path());
        auto item = Item::create(node, a->Id(), _type, a->isAvailable());
        item->setScale(0.f);
        item->setRotation(rotation);
        this->addChild(item);
        _items.push_back(item);
    }

    //TODO add arrows

    return true;
}

void EquipmentScene::Selector::_setItem(int index, float mainScale, float mainDuration, float scale, float duration) {
    if (_action == Action::Busy) {
        return;
    }

    _changeAction(Action::Busy);

    _index = index;

    for (auto i = 0; i < _items.size(); i++) {
        Vec2 pos;
        float s;
        FiniteTimeAction *action;
        float dur;
        if (i == _index) {
            s = mainScale;
            dur = mainDuration;
            pos = _center;
            action = CallFunc::create([&]() {
                auto controls = dynamic_cast<UIControls *>(this->getParent());
                switch (_type) {
                    case Type::Arrow: {
                        controls->getHero()->changeArrow(_index);
                        break;
                    }
                    case Type::Bow: {
                        controls->getHero()->changeBow(_index);
                        break;
                    }
                    case Type::Hat: {
                        controls->getHero()->changeHat(_index);
                        break;
                    }
                }
            });

        } else {
            s = scale - std::abs(_index - i) / 2.f;
            s = s < 0.f ? 0.f : 0.4f;
            dur = duration;
            auto delta = std::abs(_index - i) == 1 ? Vec2((_center.x - _begin.x) / 2, (_center.y - _begin.y) / 2)
                                                   : Vec2::ZERO;
            if (i < _index) {
                pos = _begin + delta;
            } else {
                pos = _end - delta;
            }

            action = CallFunc::create([&]() {

            });

        }

        _items[i]->runAction(
                Sequence::create(
                        Spawn::create(
                                MoveTo::create(dur, pos),
                                ScaleTo::create(dur, s),
                                NULL
                        ),
                        action,
                        NULL)
        );
    }
    this->runAction(Sequence::createWithTwoActions(
            DelayTime::create(duration),
            CallFunc::create([&]() {
                _changeAction(Action::Idle);
            })
    ));
}

bool EquipmentScene::Selector::checkTouch(Vec2 touch) {
    if (_isFocused)
        return _rect.containsPoint(touch);
    else
        return _rectFocused.containsPoint(touch);
}

EquipmentScene::Type EquipmentScene::Selector::getType() {
    return _type;
}

void EquipmentScene::Selector::onFocused(bool focused) {
    if (_isFocused == focused) {
        return;
    }

    auto controls = dynamic_cast<UIControls *>(this->getParent());
    auto shoulders = controls->getHero()->getShoulders();
    _isFocused = focused;

    if (_isFocused) {
        FiniteTimeAction *setAnimation = CallFunc::create([&]() {
            shoulders->getAnimation().fadeIn(animationName(_type), -1.f, 1);
        });

        FiniteTimeAction *setPoints = CallFunc::create([&, controls]() {

            auto transform = controls->getHero()->getSlotPosition(boneName(_type));

            _center = Vec2(transform.x, transform.y);

            _rectFocused = _getFocusedRect();

            _begin = _getBegin(_rectFocused);
            _end = _getEnd(_rectFocused);
        });

        FiniteTimeAction *setItems = CallFunc::create([&]() {
            _setItem(_index, 1.f, 0.f, 1.f, .6f);
        });

        this->runAction(Sequence::create(
                setAnimation,
                DelayTime::create(1.f),
                setPoints,
                setItems,
                NULL));

    } else {
        _setItem(_index, 0.f, 0.f, 0.f, .6f);
        shoulders->getAnimation().fadeIn("equipment_idle", -1.f, 1);
    }
}

void EquipmentScene::Selector::_changeAction(EquipmentScene::Selector::Action action) {
    if (_action != action) {
        _action = action;
    }
}

EquipmentScene::Item *EquipmentScene::Item::create(Node *view, int ind, Type type, bool isAvailable = true) {
    Item *ret = new(std::nothrow) Item();
    if (ret && ret->init(view, ind, type, isAvailable)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentScene::Item::init(Node *view, int ind, Type type, bool isAvailable) {
    if (!Sprite::init()) {
        return false;
    }
    this->addChild(view);

    _index = ind;
    _available = isAvailable;
    _type = type;

    if (!_available) {
        auto coins_bar = cocos2d::ui::Button::create();
        coins_bar->loadTextureNormal(Variables::GREEN_BUTTON, cocos2d::ui::Widget::TextureResType::PLIST);

        coins_bar->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
            switch (type) {
                case cocos2d::ui::Widget::TouchEventType::ENDED: {
                    if (true) {
                        auto item = dynamic_cast<Item *>(this->getParent());
                        item->setAvailable(_type, _index);
                        this->removeFromParent();
                    }
                }
                    break;
                default:
                    break;
            }
        });
        coins_bar->setScale(0.5f);

        //TODO set price
        auto coins = 1000;

        //TODO set currency
        auto price = cocos2d::Label::createWithTTF(StringUtils::toString(coins), Variables::FONT_NAME,
                                                   Variables::FONT_SIZE(), Size(3 * coins_bar->getContentSize().width / 5,
                                                                              Variables::FONT_SIZE()));
        price->setHorizontalAlignment(TextHAlignment::RIGHT);
        price->setColor(Color3B::WHITE);
        price->setAnchorPoint(Vec2(0, 0));
        price->setPosition(15.f, coins_bar->getContentSize().height / 2 - Variables::FONT_SIZE() / 2);

        coins_bar->addChild(price, 5);

        this->addChild(coins_bar);
    }

    return true;
}

int EquipmentScene::Item::getIndex() {
    return _index;
}

int EquipmentScene::Item::isAvailable() {
    return _available;
}

void EquipmentScene::Item::setAvailable(Type type, int index) {
    std::string key;

    switch (type) {
        case Type::Arrow: {
            key = "arrows";
            break;
        }
        case Type::Bow: {
            key = "bows";
            break;
        }
        case Type::Hat: {
            key = "hats";
            break;
        }
    }

    JSONParser::setAssetAvailable(key, index);
}

EquipmentScene::SelectorHorizontal *
EquipmentScene::SelectorHorizontal::create(Rect rect, Vec2 center, float rotation, int index, Type type) {
    SelectorHorizontal *ret = new(std::nothrow) SelectorHorizontal();
    if (ret && ret->init(rect, center, rotation, index, type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentScene::SelectorHorizontal::init(Rect rect, Vec2 center, float rotation, int index, Type type) {
    return Selector::init(rect, center, rotation, index, type);
}

Vec2 EquipmentScene::SelectorHorizontal::_getBegin(Rect rect) {
    return Vec2(
            rect.getMinX(),
            _center.y);
}

Vec2 EquipmentScene::SelectorHorizontal::_getEnd(Rect rect) {
    return Vec2(
            rect.getMaxX(),
            _center.y);
}

void EquipmentScene::SelectorHorizontal::scroll(int x, int y) {
    int dx = x > 0 ? 1 : -1;

    if (_index + dx > _items.size() - 1 || _index + dx < 0 || std::abs(x) < 20.f) {
        return;
    }

    auto controls = dynamic_cast<UIControls *>(this->getParent());

    switch (_type) {
        case Type::Arrow: {
            controls->getHero()->changeArrow(-1);
            break;
        }
        case Type::Bow: {
            controls->getHero()->changeBow(-1);
            break;
        }
        case Type::Hat: {
            controls->getHero()->changeHat(-1);
            break;
        }
    }

    _setItem(_index + dx, 1.f, .5f, 1.f, .5f);
}

Rect EquipmentScene::SelectorHorizontal::_getFocusedRect() {
    return Rect(Vec2(_center.x - _rect.size.width / 2, _center.y - _rect.size.height / 2),
                Size(_rect.size.width,
                     _rect.size.height));;
}


EquipmentScene::SelectorVertical *
EquipmentScene::SelectorVertical::create(Rect rect, Vec2 center, float rotation, int index, Type type) {
    SelectorVertical *ret = new(std::nothrow) SelectorVertical();
    if (ret && ret->init(rect, center, rotation, index, type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentScene::SelectorVertical::init(Rect rect, Vec2 center, float rotation, int index, Type type) {
    return Selector::init(rect, center, rotation, index, type);
}

Vec2 EquipmentScene::SelectorVertical::_getBegin(Rect rect) {
    return Vec2(
            _center.x,
            rect.getMinY());
}

Vec2 EquipmentScene::SelectorVertical::_getEnd(Rect rect) {
    return Vec2(
            _center.x,
            2 * _center.y - rect.getMinY());
}

void EquipmentScene::SelectorVertical::scroll(int x, int y) {
    int dy = y > 0 ? 1 : -1;

    if (_index + dy > _items.size() - 1 || _index + dy < 0 || std::abs(y) < 20.f) {
        return;
    }
    auto controls = dynamic_cast<UIControls *>(this->getParent());

    switch (_type) {
        case Type::Arrow: {
            controls->getHero()->changeArrow(-1);
            break;
        }
        case Type::Bow: {
            controls->getHero()->changeBow(-1);
            break;
        }
        case Type::Hat: {
            controls->getHero()->changeHat(-1);
            break;
        }
    }

    _setItem(_index + dy, 1.f, .5f, 1.f, .5f);
}

Rect EquipmentScene::SelectorVertical::_getFocusedRect() {
    return Rect(Vec2(_center.x - _rect.size.width, _center.y - _rect.size.height),
                Size(_rect.size.width,
                     2 *_rect.size.height));;
}
