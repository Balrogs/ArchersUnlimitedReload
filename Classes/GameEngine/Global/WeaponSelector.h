//
// Created by igor on 08.09.16.
//

#ifndef ARCUNLIM_WEAPONSELECTOR_H
#define ARCUNLIM_WEAPONSELECTOR_H

#include <GameEngine/Objects/Hero.h>
#include "cocos2d.h"

class WeaponSelector : public cocos2d::Node {
private:

    class SelectorItem {
    private:
        cocos2d::Sprite *_view;
        float _radius;
    public:
        SelectorItem(const char *name, float scale);

        cocos2d::Sprite *getView();

        float getRadius();
    };

    std::vector<SelectorItem *> _items;

    bool checkCollisions(double angle, SelectorItem *node);

    cocos2d::Size _SIZE;

    Hero *_hero;

    cocos2d::Rect *_box;

    float _radius;

    void _goNext(int dest);

    bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event);

    bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event);

    bool circlesIntersect(cocos2d::Vec2 pos1, float rad1, cocos2d::Vec2 pos2, float rad2);

public:

    WeaponSelector(Hero *hero);

    ~WeaponSelector();

    cocos2d::Rect getBoundingBox();


};

#endif //ARCUNLIM_WEAPONSELECTOR_H
