//
// Created by igor on 16.12.16.
//

#ifndef ARCHERSUNLIMITED_BOX_H
#define ARCHERSUNLIMITED_BOX_H

#include "cocos2d.h"

USING_NS_CC;

class Box : public Node {
public:
    static Box* create(int type);
    bool init(int type);
    void breakBox();
private:
    int _hp;
    bool _isBreakble;
    bool _isBroken;
    bool _isStatic;
    Sprite* _box;
    Sprite* _brokenBox;
};


#endif //ARCHERSUNLIMITED_BOX_H
