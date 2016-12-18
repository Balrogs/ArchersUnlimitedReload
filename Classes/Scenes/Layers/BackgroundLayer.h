//
// Created by igor on 13.10.16.
//

#ifndef ARCUNLIM_BACKGROUNDLAYER_H
#define ARCUNLIM_BACKGROUNDLAYER_H

#include "cocos2d.h"

class PointObject : public cocos2d::Ref {
public:
    inline void setRation(cocos2d::Point ratio) { _ratio = ratio; }

    inline void setOffset(cocos2d::Point offset) { _offset = offset; }

    inline void setChild(cocos2d::Node *var) { _child = var; }

    inline cocos2d::Point getOffset() const { return _offset; }

    inline cocos2d::Node *getChild() const { return _child; }

private:
    cocos2d::Point _ratio;
    cocos2d::Point _offset;
    cocos2d::Node *_child;
};

class InfiniteParallaxNode : public cocos2d::ParallaxNode {
public:

    static InfiniteParallaxNode *create(int id);

    bool init(int id);

    void updatePosition();

    void createGraveyard();

    void createDesert();

    void createGreenfield();
private:
    float GROUND;
    cocos2d::Size _size;
    cocos2d::DrawNode* _ground;
};


class BackgroundLayer : public cocos2d::LayerColor {
public:

    bool init();

    static BackgroundLayer *create();

    bool init(int id);

    static BackgroundLayer *create(int id);

    void setPosition(float x, float y) override;

    cocos2d::Vec3 getPosition3D() const override;

    void removeSprites();

    void move(float delta);

    void addArrow(Node* arrow);

protected:
    InfiniteParallaxNode *_parallax;
    int _id;
};


#endif //ARCUNLIM_BACKGROUNDLAYER_H
