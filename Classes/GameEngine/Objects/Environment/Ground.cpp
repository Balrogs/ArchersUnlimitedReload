//
// Created by igor on 02.11.16.
//

#include <Scenes/PlayLayers/Battle.h>
#include "Ground.h"

USING_NS_CC;

Ground::Ground(float y_pos, float width) {

    this->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(0.f, y_pos),
                                                          Vec2(width, y_pos)));
    this->getPhysicsBody()->setContactTestBitmask(true);

    float curr_width = 0;

    while(curr_width < width){
        auto random = RandomHelper::random_int(1,3);
        auto path = StringUtils::format("Grass%d.png", random);
        auto grass = cocos2d::Sprite::createWithSpriteFrameName(path);
        grass->setScaleX(RandomHelper::random_int(0,1) == 0?-1:1);
        grass->setPosition(curr_width, y_pos);
        curr_width+= grass->getContentSize().width - 100.f;
        this->addChild(grass);
    }

}