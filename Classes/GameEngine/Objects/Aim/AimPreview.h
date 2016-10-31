//
// Created by igor on 20.09.16.
//

#ifndef ARCUNLIM_AIMPREVIEW_H
#define ARCUNLIM_AIMPREVIEW_H


#include <cocos2d.h>

class AimPreview : public cocos2d::Node {

public:
    AimPreview(float scale, const cocos2d::Vec2 &position);

    ~AimPreview();

    void update(float scale, const cocos2d::Vec2 &position);
    void remove();
};


#endif //ARCUNLIM_AIMPREVIEW_H
