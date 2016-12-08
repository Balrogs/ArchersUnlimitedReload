//
// Created by igor on 02.11.16.
//

#include <Scenes/PlayLayers/Battle.h>
#include "Ground.h"

USING_NS_CC;


bool Ground::init(float y_pos, float width, std::string path) {
    if (!Node::init()) {
        return false;
    }
    this->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(0.f, y_pos),
                                                        Vec2(width, y_pos)));
    this->getPhysicsBody()->setContactTestBitmask(true);

    auto visibleSize = Director::getInstance()->getVisibleSize();

    float curr_width = - visibleSize.width * 5;

    while (curr_width < width) {
        auto grass = cocos2d::Sprite::createWithSpriteFrameName(path);
        grass->setPosition(curr_width, grass->getContentSize().height / 2);
        curr_width += grass->getContentSize().width - 3.f;
        this->addChild(grass);
    }
    return true;
}

Ground *Ground::create(float y_pos, float width, std::string path) {
    Ground *ret = new(std::nothrow) Ground();
    if (ret && ret->init(y_pos, width, path)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool Clouds::init(float width, std::string path) {
    if (!Node::init()) {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();

    float curr_width = - visibleSize.width * 2;

    while (curr_width < width) {
        auto grass = cocos2d::Sprite::createWithSpriteFrameName(path);
        grass->setPosition(curr_width, visibleSize.height - grass->getContentSize().height / 2);
        curr_width += grass->getContentSize().width- 3.f;
        this->addChild(grass);
    }
    return true;
}

Clouds *Clouds::create(float width, std::string path) {
    Clouds *ret = new(std::nothrow) Clouds();
    if (ret && ret->init(width, path)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}