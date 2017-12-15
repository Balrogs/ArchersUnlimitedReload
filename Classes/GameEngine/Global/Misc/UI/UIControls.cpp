#include <GameEngine/Global/Misc/JSONParser.h>
#include <Scenes/MenuLayers/Main/EquipmentScene.h>
#include <GameEngine/Global/Variables.h>
#include "UIControls.h"


std::string Item::boneName(Item::Type v) {
    switch (v) {
        case Arrow:
            return "arrow";
        case Bow:
            return "bow";
        case Hat:
            return "head";
    }
}

std::string Item::animationName(Item::Type v) {
    switch (v) {
        case Arrow:
            return Variables::EQUIPMENT_ARROW_ANIMATION;
        case Bow:
            return Variables::EQUIPMENT_BOW_ANIMATION;
        case Hat:
            return Variables::STICKMAN_IDLE_ANIMATION;
    }
}

UIControls *UIControls::create(HeroPreview *hero) {
    UIControls *ret = new(std::nothrow) UIControls();
    if (ret && ret->init(hero)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UIControls::init(HeroPreview *hero) {
    if (!Node::init()) {
        return false;
    }
    _touch = -1;
    _hero = hero;

    auto heroHeight = _hero->getHatHeight();

    auto width = Director::getInstance()->getVisibleSize().width * 0.1f;


    auto arrowSelectorArea = Rect(Vec2(-width, 0), Size(width, 2 * heroHeight));
    auto hatSelectorArea = Rect(Vec2(-width, heroHeight), Size(2 * width, heroHeight * 0.7f));
    auto bowSelectorArea = Rect(Vec2::ZERO, Size(width, 2 * heroHeight));

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
                                                  arrowCenter,
                                                  -180,
                                                  _hero->getPlayerView()->getArrow()->Id(),
                                                  Item::Type::Arrow);
    this->addChild(arrowSelector);


    auto bowTransform = _hero->getSlotPosition("bow");
    auto bowPoint = Vec3(bowTransform.x, bowTransform.y, 0);
    _hero->getWorldToNodeTransform().translate(bowPoint);
    auto bowCenter = Vec2(bowPoint.x, bowPoint.y);
    auto bowSelector = SelectorVertical::create(bowSelectorArea,
                                                bowCenter,
                                                0,
                                                _hero->getPlayerView()->getBow()->Id(),
                                                Item::Type::Bow);
    this->addChild(bowSelector);

    auto hatTransform = _hero->getSlotPosition("head");
    auto hatPoint = Vec3(hatTransform.x, -hatTransform.y, 0);
    _hero->getNodeToWorldTransform().translate(hatPoint);
    auto hatCenter = Vec2(hatPoint.x, hatPoint.y);

    auto hatSelector = SelectorHorizontal::create(hatSelectorArea,
                                                  hatCenter,
                                                  0,
                                                  _hero->getPlayerView()->getHat()->Id(),
                                                  Item::Type::Hat);
    this->addChild(hatSelector);


    auto hatTrigger = SelectorTrigger::create(hatSelector);
    hatTrigger->setPosition(_hero->getSlotPosition(Item::boneName(Item::Type::Hat)));
    this->addChild(hatTrigger);
    _triggers.push_back(hatTrigger);


    auto arrowTrigger = SelectorTrigger::create(arrowSelector);
    arrowTrigger->setPosition(_hero->getSlotPosition(Item::boneName(Item::Type::Arrow)));
    this->addChild(arrowTrigger);
    _triggers.push_back(arrowTrigger);


    auto bowTrigger = SelectorTrigger::create(bowSelector);
    bowTrigger->setPosition(_hero->getSlotPosition(Item::boneName(Item::Type::Bow)));
    this->addChild(bowTrigger);
    _triggers.push_back(bowTrigger);

    const auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(UIControls::_touchHandlerBegin, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(UIControls::_touchHandlerMove, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(UIControls::_touchHandlerEnd, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);


    return true;
}

HeroPreview *UIControls::getHero() {
    return _hero;
}

bool UIControls::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_touch < 0) {
        _touch = touch->getID();
    } else {
        return false;
    }
    if(!_busy){
        for(auto t : _triggers){
            if(t->isTouched(touch->getLocation())){
                _setSelector(t);
            }
        }
    }
    return true;

}

bool UIControls::_touchHandlerMove(const cocos2d::Touch *touch, cocos2d::Event *event) {
    return _touch == touch->getID();

}

bool UIControls::_touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_touch == touch->getID()) {
        _touch = -1;
    } else {
        return false;
    }
    if (_focusedTrigger == nullptr) {
        return true;
    }

    const auto start = touch->getStartLocation();
    const auto curr = touch->getLocation();
    int x = (int) (start.x - curr.x);
    int y = (int) (start.y - curr.y);
    _focusedTrigger->getSelector()->scroll(x, y);

    return true;
}

void UIControls::triggerSelector(int id, int assetId) {
    _triggers[id]->getSelector()->setIndex(assetId);
    _setSelector(_triggers[id]);
}

void UIControls::_setSelector(SelectorTrigger *trigger) {
    _busy = true;

    auto hidePrevSelector = CallFunc::create([&](){
        if(_focusedTrigger != nullptr){
            _focusedTrigger->resume();
        }
    });

    auto delay = 0.f;
    if(_focusedTrigger != nullptr){
        delay = .7f;
    }

    auto showNewSelector = CallFunc::create([&, trigger](){
        _focusedTrigger = trigger;
        trigger->stop();
    });

    auto setFree = CallFunc::create([&](){
        _busy = false;
    });

    this->runAction(Sequence::create(
            hidePrevSelector,
            DelayTime::create(delay),
            showNewSelector,
            DelayTime::create(1.4f),
            setFree,
            NULL
    ));
}

bool Selector::init(Rect rect, Vec2 center, float rotation, int index, Item::Type type) {
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
        case Item::Type::Arrow: {
            asset = "arrows";
            break;
        }
        case Item::Type::Bow: {
            asset = "bows";
            break;
        }
        case Item::Type::Hat: {
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

    return true;
}

void Selector::_setItem(int index, float mainScale, float mainDuration, float scale, float duration) {
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
                    case Item::Type::Arrow: {
                        controls->getHero()->changeArrow(_index);
                        break;
                    }
                    case Item::Type::Bow: {
                        controls->getHero()->changeBow(_index);
                        break;
                    }
                    case Item::Type::Hat: {
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


Item::Type Selector::getType() {
    return _type;
}

void Selector::onFocused(bool focused) {
    if (_isFocused == focused) {
        return;
    }

    auto controls = dynamic_cast<UIControls *>(this->getParent());
    auto shoulders = controls->getHero()->getShoulders();
    _isFocused = focused;

    if (_isFocused) {
        FiniteTimeAction *setAnimation = CallFunc::create([&, shoulders]() {
            shoulders->getAnimation().fadeIn(Item::animationName(_type), -1.f, 1);
        });

        FiniteTimeAction *setPoints = CallFunc::create([&, controls]() {

            auto transform = controls->getHero()->getSlotPosition(Item::boneName(_type));

            _center = Vec2(transform.x, transform.y);

            _rectFocused = _getFocusedRect();

            _begin = _getBegin(_rectFocused);
            _end = _getEnd(_rectFocused);
        });

        FiniteTimeAction *setItems = CallFunc::create([&]() {
            _setItem(_index, 1.f, 0.f, 1.f, .6f);
            _addArrows();
        });

        this->runAction(Sequence::create(
                setAnimation,
                DelayTime::create(1.f),
                setPoints,
                setItems,
                NULL));

    } else {
        _setItem(_index, 0.f, 0.f, 0.f, .6f);
        auto lastAnimation = shoulders->getAnimation().getLastAnimationName();
        if(lastAnimation != Variables::STICKMAN_IDLE_ANIMATION){
            shoulders->getAnimation().fadeIn(lastAnimation + "_reverse", -1.f, 1);
        }
        _removeArrows();
    }
}

void Selector::_changeAction(Selector::Action action) {
    if (_action != action) {
        _action = action;
    }
}

void Selector::_removeArrows() {
    for(auto child : this->getChildren()){
        if(child->getTag() == 1){
            child->removeFromParent();
        }
    }
}

void Selector::setIndex(int index) {
    _index = index;
}

Item *Item::create(Node *view, int ind, Type type, bool isAvailable = true) {
    Item *ret = new(std::nothrow) Item();
    if (ret && ret->init(view, ind, type, isAvailable)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool Item::init(Node *view, int ind, Type type, bool isAvailable) {
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
                    if (_available) {
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

int Item::getIndex() {
    return _index;
}

bool Item::isAvailable() {
    return _available;
}

void Item::setAvailable(Type type, int index) {
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


SelectorHorizontal *
SelectorHorizontal::create(Rect rect, Vec2 center, float rotation, int index, Item::Type type) {
    SelectorHorizontal *ret = new(std::nothrow) SelectorHorizontal();
    if (ret && ret->init(rect, center, rotation, index, type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SelectorHorizontal::init(Rect rect, Vec2 center, float rotation, int index, Item::Type type) {
    return Selector::init(rect, center, rotation, index, type);
}

Vec2 SelectorHorizontal::_getBegin(Rect rect) {
    return Vec2(
            rect.getMinX(),
            _center.y);
}

Vec2 SelectorHorizontal::_getEnd(Rect rect) {
    return Vec2(
            rect.getMaxX(),
            _center.y);
}

void SelectorHorizontal::scroll(int x, int y) {
    int dx = x > 0 ? 1 : -1;

    if (_index + dx > _items.size() - 1 || _index + dx < 0 || std::abs(x) < 20.f) {
        return;
    }

    auto controls = dynamic_cast<UIControls *>(this->getParent());

    switch (_type) {
        case Item::Type::Arrow: {
            controls->getHero()->changeArrow(-1);
            break;
        }
        case Item::Type::Bow: {
            controls->getHero()->changeBow(-1);
            break;
        }
        case Item::Type::Hat: {
            controls->getHero()->changeHat(-1);
            break;
        }
    }

    _setItem(_index + dx, 1.f, .5f, 1.f, .5f);
}

Rect SelectorHorizontal::_getFocusedRect() {
    return Rect(Vec2(_center.x - _rect.size.width / 2, _center.y - _rect.size.height / 2),
                Size(_rect.size.width,
                     _rect.size.height));
}

void SelectorHorizontal::_addArrows() {
    auto up = Sprite::createWithSpriteFrameName(Variables::UI_ARROW);
    auto down = Sprite::createWithSpriteFrameName(Variables::UI_ARROW);

    up->setScale(0.5f);
    down->setScale(0.5f);

    up->setPosition(Vec2(
            _rect.getMaxX() - up->getContentSize().width / 3,
            _rect.getMaxY()
    ));
    down->setPosition(Vec2(
            _rect.getMinX() + down->getContentSize().width / 3,
            _rect.getMaxY()
    ));

    up->setRotation(180);
    down->setRotation(0);

    this->addChild(up, 0, 1);
    this->addChild(down, 0, 1);
}


SelectorVertical *
SelectorVertical::create(Rect rect, Vec2 center, float rotation, int index, Item::Type type) {
    SelectorVertical *ret = new(std::nothrow) SelectorVertical();
    if (ret && ret->init(rect, center, rotation, index, type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SelectorVertical::init(Rect rect, Vec2 center, float rotation, int index, Item::Type type) {
    return Selector::init(rect, center, rotation, index, type);
}

Vec2 SelectorVertical::_getBegin(Rect rect) {
    return Vec2(
            _center.x,
            rect.getMinY());
}

Vec2 SelectorVertical::_getEnd(Rect rect) {
    return Vec2(
            _center.x,
            rect.getMaxY());
}

void SelectorVertical::scroll(int x, int y) {
    int dy = y > 0 ? 1 : -1;

    if (_index + dy > _items.size() - 1 || _index + dy < 0 || std::abs(y) < 20.f) {
        return;
    }
    auto controls = dynamic_cast<UIControls *>(this->getParent());

    switch (_type) {
        case Item::Type::Arrow: {
            controls->getHero()->changeArrow(-1);
            break;
        }
        case Item::Type::Bow: {
            controls->getHero()->changeBow(-1);
            break;
        }
        case Item::Type::Hat: {
            controls->getHero()->changeHat(-1);
            break;
        }
    }

    _setItem(_index + dy, 1.f, .5f, 1.f, .5f);
}

Rect SelectorVertical::_getFocusedRect() {
    return Rect(Vec2(_center.x - _rect.size.width / 2, _center.y - _rect.size.height / 2),
                Size(_rect.size.width,
                     _rect.size.height));
}

void SelectorVertical::_addArrows() {
    auto up = Sprite::createWithSpriteFrameName(Variables::UI_ARROW);
    auto down = Sprite::createWithSpriteFrameName(Variables::UI_ARROW);

    up->setScale(0.5f);
    down->setScale(0.5f);

    auto xPos = 0.f;
    switch(_type){
        case  Item::Type::Arrow : {
            xPos = _rect.getMinX();
            break;
        }
        case  Item::Type::Bow : {
            xPos = _rect.getMaxX();
            break;
        }
        default : {
            xPos = _center.x;
            break;
        }
    }

    up->setPosition(Vec2(
            xPos,
            _rect.getMaxY() - up->getContentSize().height / 3
    ));
    down->setPosition(Vec2(
            xPos,
            _rect.getMinY() + down->getContentSize().height / 3
    ));

    up->setRotation(90);
    down->setRotation(-90);

    this->addChild(up, 0, 1);
    this->addChild(down, 0, 1);
}

SelectorTrigger *SelectorTrigger::create(Selector *selector) {
    SelectorTrigger *ret = new(std::nothrow) SelectorTrigger();
    if (ret && ret->init(selector)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SelectorTrigger::init() {
    if(!Node::init()){
        return false;
    }
    _isStopped = false;

    auto dot = Sprite::createWithSpriteFrameName(Variables::POINT);
    this->addChild(dot);

    _rectangle = Rect(dot->getBoundingBox());

    dot->runAction(RepeatForever::create(
            Sequence::create(
                    ScaleTo::create(1.f, 0.4f),
                    ScaleTo::create(1.f, 0.8f),
                    NULL
            )
    ));

    return true;
}

bool SelectorTrigger::isTouched(Vec2 touch) {
    Vec3 p3 = Vec3(touch.x, touch.y, 0);
    this->getWorldToNodeTransform().transformPoint(&p3);
    Vec2 point = Vec2(p3.x, p3.y);
    return !_isStopped && _rectangle.containsPoint(point);
}

void SelectorTrigger::stop() {
    if(!_isStopped){
        _isStopped = true;
        this->setVisible(false);
        _selector->onFocused(_isStopped);
    }
}

void SelectorTrigger::resume() {
    if(_isStopped) {
        _isStopped = false;
        this->setVisible(true);
        _selector->onFocused(_isStopped);
    }
}

bool SelectorTrigger::init(Selector *selector) {
    _selector = selector;
    return SelectorTrigger::init();
}

Selector *SelectorTrigger::getSelector() {
    return _selector;
}

