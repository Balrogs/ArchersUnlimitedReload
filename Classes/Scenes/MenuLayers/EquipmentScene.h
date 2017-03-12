#ifndef ARCUNLIM_EQUIPMENTSCENE_H
#define ARCUNLIM_EQUIPMENTSCENE_H

#include <GameEngine/Objects/Hero.h>
#include <ui/UIButton.h>
#include "cocos2d.h"

USING_NS_CC;

class EquipmentScene : public Layer {
public:

    static EquipmentScene* getInstance();

    CREATE_FUNC(EquipmentScene);

    bool init();

    cocos2d::Vec2 getButtonPosition();

    void resumeEquipment();

    dragonBones::CCFactory factory;

protected:

    enum Type {
        Arrow,
        Bow,
        Hat
    };

    static std::string boneName(Type v);

    static std::string  animationName(Type v);

    class Item : public Sprite {
    public:
        static Item *create(Node* view, int ind, Type type, bool isAvailable);

        bool init(Node* view, int ind, Type type, bool isAvailable);
        int getIndex();
        int isAvailable();
        void setAvailable(Type type, int index);

    protected:
        int _index;
        bool _available;
        Type _type;
    };

    class Selector : public Node {
        enum Action{
            Busy,
            Idle
        };
    public:
        virtual bool init(Rect rect, Vec2 center, float rotation, int index, Type type);

        virtual void scroll(int x, int y) = 0;

        void onFocused(bool focused);

        bool checkTouch(Vec2 touch);

        Type getType();

    protected:
        int _index;
        Vec2 _center;
        Vec2 _begin;
        Vec2 _end;
        std::vector<Item*> _items;
        Rect _rect;
        Rect _rectFocused;
        Type _type;
        Action _action;
        bool _isFocused;

        virtual Vec2 _getBegin(Rect rect) = 0;
        virtual Vec2 _getEnd(Rect rect) = 0;
        virtual Rect _getFocusedRect() = 0;

        void _setItem(int index, float mainScale, float mainDuration, float scale, float duration);

        void _changeAction(Action action);
    };

    class SelectorHorizontal : public Selector {
    public:
        static SelectorHorizontal *create(Rect rect, Vec2 center, float rotation, int index, Type type);

        bool init(Rect rect, Vec2 center, float rotation, int index, Type type);
        virtual void scroll(int x, int y);
    protected:

        virtual Vec2 _getBegin(Rect rect);
        virtual Vec2 _getEnd(Rect rect);
        virtual Rect _getFocusedRect();
    };

    class SelectorVertical : public Selector {
    public:
        static SelectorVertical *create(Rect rect, Vec2 center, float rotation, int index, Type type);

        bool init(Rect rect, Vec2 center, float rotation, int index, Type type);

        virtual void scroll(int x, int y);
    protected:

        virtual Vec2 _getBegin(Rect rect);
        virtual Vec2 _getEnd(Rect rect);
        virtual Rect _getFocusedRect();
    };

    class UIControls : public Node {
    public:
        static UIControls *create(HeroPreview* hero);

        bool init(HeroPreview* hero);

        HeroPreview* getHero();

    protected:
        HeroPreview* _hero;
        std::vector<Selector*> _selectors;
        int _touch;

        Selector* _focused;
        Selector* _getFocused(Vec2 pos);

        bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event);

        bool _touchHandlerMove(const cocos2d::Touch *touch, cocos2d::Event *event);

        bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event);
    };

    static EquipmentScene *_instance;

    Size _visibleSize;

    UIControls* _controls;
    HeroPreview* _hero;
    Node* _stand;

    Label* _hatsCount;
    Label* _arrowsCount;
    Label* _bowsCount;

    void _readAssets();

    void _enterFrameHandler(float passedTime);

    void onQuit();
};
#endif //ARCUNLIM_EQUIPMENTSCENE_H
