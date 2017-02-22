#ifndef ARCHERSUNLIMITED_MAINCLIP_H
#define ARCHERSUNLIMITED_MAINCLIP_H

#include "cocos2d.h"
USING_NS_CC;

class MainClip : public Sprite {
public:

    static MainClip *create();

    bool init();

    void update(float dt);

protected:
    std::map<Node*, float> _objects;
    float _starPos;
    float _endPos;

    float _totalWay;
};


#endif //ARCHERSUNLIMITED_MAINCLIP_H
