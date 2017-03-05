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

    void resumeEquipment();

protected:

    enum Alignment{
        Vertical,
        Horizontal
    };

    class Item : public Sprite {
    public:
        static Item *create(Node* view, int ind, bool isAvailable);

        bool init(Node* view, int ind, bool isAvailable);

    protected:
        int _index;
        bool _available;
    };

    class Selector : public Node {
    public:
        static Selector *create(Alignment alignment);

        bool init(Alignment alignment);

    protected:
        int _index;
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
    Node* _stand;

    Label* _hatsCount;
    Label* _arrowsCount;
    Label* _bowsCount;

    void _readAssets();

    void onQuit();
};
#endif //ARCUNLIM_EQUIPMENTSCENE_H
