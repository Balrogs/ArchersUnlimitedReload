#ifndef ARCHERSUNLIMITED_UICONTROLS_H
#define ARCHERSUNLIMITED_UICONTROLS_H

#include <GameEngine/Objects/Hero.h>
#include <ui/UIButton.h>
#include "cocos2d.h"

USING_NS_CC;



class Item : public Sprite {
public:
    enum Type {
        Arrow,
        Bow,
        Hat
    };

    static std::string boneName(Item::Type v);

    static std::string  animationName(Item::Type v);

    static Item *create(Node* view, int ind, Item::Type type, bool isAvailable);

    bool init(Node* view, int ind, Item::Type type, bool isAvailable);
    int getIndex();
    bool isAvailable();
    void setAvailable(Item::Type type, int index);

protected:
    int _index;
    bool _available;
    Item::Type _type;
};

class Selector : public Node {

    enum Action{
        Busy,
        Idle
    };

public:
    virtual bool init(Rect rect, Vec2 center, float rotation, int index, Item::Type type);

    virtual void scroll(int x, int y) = 0;

    void onFocused(bool focused);

    Item::Type getType();

protected:
    int _index;
    Vec2 _center;
    Vec2 _begin;
    Vec2 _end;
    std::vector<Item*> _items;
    Rect _rect;
    Rect _rectFocused;
    Item::Type _type;
    Action _action;
    bool _isFocused;

    virtual Vec2 _getBegin(Rect rect) = 0;
    virtual Vec2 _getEnd(Rect rect) = 0;
    virtual Rect _getFocusedRect() = 0;
    virtual void _addArrows() = 0;
    virtual void _removeArrows();

    void _setItem(int index, float mainScale, float mainDuration, float scale, float duration);

    void _changeAction(Action action);
};

class SelectorHorizontal : public Selector {
public:
    static SelectorHorizontal *create(Rect rect, Vec2 center, float rotation, int index, Item::Type type);

    bool init(Rect rect, Vec2 center, float rotation, int index, Item::Type type);
    virtual void scroll(int x, int y);
protected:

    virtual Vec2 _getBegin(Rect rect);
    virtual Vec2 _getEnd(Rect rect);
    virtual Rect _getFocusedRect();
    virtual void _addArrows();
};

class SelectorVertical : public Selector {
public:
    static SelectorVertical *create(Rect rect, Vec2 center, float rotation, int index, Item::Type type);

    bool init(Rect rect, Vec2 center, float rotation, int index, Item::Type type);

    virtual void scroll(int x, int y);
protected:

    virtual Vec2 _getBegin(Rect rect);
    virtual Vec2 _getEnd(Rect rect);
    virtual Rect _getFocusedRect();
    virtual void _addArrows();
};

class SelectorTrigger : public Node {
public:

    static SelectorTrigger* create(Selector* selector);

    bool init();
    bool init(Selector* selector);

    Selector* getSelector();

    bool isTouched(Vec2 touch);

    void stop();
    void resume();

protected:
    Selector* _selector;
    bool _isStopped;
    Rect _rectangle;
};

class UIControls : public Node {
public:
    static UIControls *create(HeroPreview* hero);

    bool init(HeroPreview* hero);

    HeroPreview* getHero();

protected:
    HeroPreview* _hero;
    std::vector<SelectorTrigger*> _triggers;
    int _touch;
    bool _busy;

    SelectorTrigger* _focusedTrigger;

    bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event);

    bool _touchHandlerMove(const cocos2d::Touch *touch, cocos2d::Event *event);

    bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event);
};


#endif //ARCHERSUNLIMITED_UICONTROLS_H
