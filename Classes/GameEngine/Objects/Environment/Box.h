//
// Created by igor on 16.12.16.
//

#ifndef ARCHERSUNLIMITED_BOX_H
#define ARCHERSUNLIMITED_BOX_H

#include "cocos2d.h"

USING_NS_CC;

enum BoxType {
    WOODEN_BOX,
    WOODEN_TRIANGLE,
    STONE_LARGE_BOX,
    STONE_MEDIUM_BOX,
    STONE_SMALL_BOX,
    TNT
};

class Box : public Node {
public:
    static Box* create(BoxType type);
    virtual bool init(BoxType type);
    virtual void breakBox();
private:
    int _hp;
    bool _isBreakble;
    bool _isBroken;
    bool _isStatic;
    Sprite* _box;
    Sprite* _brokenBox;
};


#endif //ARCHERSUNLIMITED_BOX_H
