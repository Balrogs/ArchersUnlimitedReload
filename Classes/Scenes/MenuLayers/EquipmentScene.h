#ifndef ARCUNLIM_EQUIPMENTSCENE_H
#define ARCUNLIM_EQUIPMENTSCENE_H

#include <GameEngine/Objects/Hero.h>
#include <ui/UIButton.h>
#include "cocos2d.h"

USING_NS_CC;

class EquipmentScene : public Layer {
public:
    CREATE_FUNC(EquipmentScene);

    bool init();

    cocos2d::Vec2 getButtonPosition();
protected:

    enum Alignment{
        Vertical,
        Horizontal
    };


    enum BuyType{
        Achievement,
        Money,
        Chest
    };

    class Item : public Sprite {
    public:
        static Item *create(Node* view, int ind, float value, bool isAvailable, BuyType type);

        bool init(Node* view, int ind, bool isAvailable, BuyType type, float value);

    protected:
        int _index;
        bool _available;
        Label* _message;
        ui::Button* _buy;
    };

    class Selector : public Node {
    public:
        static Selector *create(Alignment alignment);

        bool init(Alignment alignment);

    protected:
        int _index;
        Label* _message;
        ui::Button* _buy;
    };

    class UIControls : public Node {
    public:
        static UIControls *create(HeroPreview* hero);

        bool init(HeroPreview* hero);

        HeroPreview* getHero();

    protected:
        HeroPreview* _hero;
    };

    Size _visibleSize;

    UIControls* _controls;

    void onEnter() override;

    void _readAssets();

    void onQuit();
};
#endif //ARCUNLIM_EQUIPMENTSCENE_H
