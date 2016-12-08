//
// Created by igor on 13.10.16.
//

#include <GameEngine/Objects/Environment/Ground.h>
#include <Scenes/PlayLayers/Battle.h>
#include "BackgroundLayer.h"

USING_NS_CC;


InfiniteParallaxNode *InfiniteParallaxNode::create(int id) {
    InfiniteParallaxNode *ret = new(std::nothrow) InfiniteParallaxNode();
    if (ret && ret->init(id)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void InfiniteParallaxNode::updatePosition() {
    int safeOffset = -10;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // 1. For each child of an parallax node
    for (int i = 0; i < _children.size(); i++) {
        auto node = _children.at(i);
        // 2. We check whether it is out of the left side of the visible area
        if (convertToWorldSpace(node->getPosition()).x + node->getContentSize().width < safeOffset)
            // 3. Find PointObject that corresponds to current node
            for (int i = 0; i < _parallaxArray->num; i++) {
                auto po = (PointObject *) _parallaxArray->arr[i];
                // If yes increase its current offset on the value of visible width
                if (po->getChild() == node)
                    po->setOffset(po->getOffset() +
                                  Point(visibleSize.width + node->getContentSize().width, 0));
            }
    }
}

bool InfiniteParallaxNode::init(int id) {
    if (!ParallaxNode::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    Ground *hills2 = Ground::create(BattleScene::instance->GROUND, visibleSize.width * 10, "hills2.png");

    this->addChild(hills2, 1, Point(0.8, 0.8), Point::ZERO);

    Ground *hills1 = Ground::create(BattleScene::instance->GROUND, visibleSize.width * 10, "hills1.png");

    this->addChild(hills1, 2, Point(1, 1), Point::ZERO);

    switch (id) {
        case 1:
            createGraveyard();
            break;
        case 2:
            createDesert();
            break;
        case 3:
            createGreenfield();
            break;
        case 4:
            break;
        case 5:
            break;
        default:
            return false;
    }
    return true;
}

void InfiniteParallaxNode::createGraveyard() {

    auto visibleSize = Director::getInstance()->getVisibleSize();

    Clouds *cloud2 = Clouds::create(visibleSize.width * 3, "clouds2.png");

    this->addChild(cloud2, 1, Point(0.2, 0.2), Point::ZERO);

    Clouds *cloud1 = Clouds::create(visibleSize.width * 3, "clouds1.png");

    this->addChild(cloud1, 3, Point(0.1, 0.1), Point::ZERO);

    auto moon = Sprite::createWithSpriteFrameName("moon.png");
    auto y = visibleSize.height - 100.f;
    this->addChild(moon, 2, Point(0, 0), Point(visibleSize.width / 2, y));


    //graves

    unsigned int gravesQuantity = 35;
    for(unsigned int i = 0; i < gravesQuantity; i++)
    {
        auto grave = Sprite::createWithSpriteFrameName("grave" + StringUtils::toString(RandomHelper::random_int(1, 3)) +".png");
        grave->setAnchorPoint(Point::ZERO);
        grave->setScale(RandomHelper::random_real(0.3f, 1.f));
        grave->setTag(1000 + i);
        this->addChild(grave, RandomHelper::random_int(2, 3), Point(0.5, 1),
                       Point((visibleSize.width / 5) * (i + 1) + RandomHelper::random_int(0, 100),  RandomHelper::random_int(50, 150)));
    }


    //trees

    unsigned int treesQuantity = 35;
    for(unsigned int i = 0; i < treesQuantity; i++)
    {
        auto tree = Sprite::createWithSpriteFrameName("tree" + StringUtils::toString(RandomHelper::random_int(1, 2)) +".png");
        tree->setAnchorPoint(Point::ZERO);
        tree->setScale(RandomHelper::random_real(0.3f, 1.f));
        tree->setTag(1000 + i);
        this->addChild(tree, RandomHelper::random_int(2, 3), Point(0.5, 1),
                            Point((visibleSize.width / 5) * (i + 1) + RandomHelper::random_int(0, 100),  RandomHelper::random_int(50, 150)));
    }

    //fences

    unsigned int fencesQuantity = 35;
    for(unsigned int i = 0; i < fencesQuantity; i++)
    {
        auto fence = Sprite::createWithSpriteFrameName("fence" + StringUtils::toString(RandomHelper::random_int(1, 2)) +".png");
        fence->setAnchorPoint(Point::ZERO);
        fence->setScale(RandomHelper::random_real(0.3f, 1.f));
        fence->setTag(1000 + i);
        this->addChild(fence, RandomHelper::random_int(2, 3), Point(0.5, 1),
                       Point((visibleSize.width / 5) * (i + 1) + RandomHelper::random_int(0, 100),  RandomHelper::random_int(50, 150)));
    }


    //grass

    unsigned int grassQuantity = 35;
    for(unsigned int i = 0; i < grassQuantity; i++)
    {
        auto grass = Sprite::createWithSpriteFrameName("grass" + StringUtils::toString(RandomHelper::random_int(1, 2)) +".png");
        grass->setAnchorPoint(Point::ZERO);
        grass->setScale(RandomHelper::random_real(0.3f, 1.f));
        grass->setTag(1000 + i);
        this->addChild(grass, RandomHelper::random_int(2, 3), Point(0.5, 1),
                       Point((visibleSize.width / 5) * (i + 1) + RandomHelper::random_int(0, 100),  RandomHelper::random_int(50, 150)));
    }

}

void InfiniteParallaxNode::createDesert() {

}

void InfiniteParallaxNode::createGreenfield() {

}

bool BackgroundLayer::init(int id) {
    Color4B bgColor;
    _id = id;
    switch (id) {
        case 1:
            bgColor = Color4B(72, 75, 80, 255);
            break;
        case 2:
            bgColor = Color4B(196, 225, 254, 255);
            break;
        case 3:
            bgColor = Color4B(148, 200, 254, 255);
            break;
        case 4:
            bgColor = Color4B(148, 200, 254, 255);
            break;
        case 5:
            bgColor = Color4B(148, 200, 254, 255);
            break;
        default:
            return false;
    }
    if (!LayerColor::initWithColor(bgColor)) {
        return false;
    }
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("background-" + StringUtils::toString(id) + ".plist");
    _parallax = InfiniteParallaxNode::create(id);
    this->addChild(_parallax, 1);

    return true;
}

BackgroundLayer *BackgroundLayer::create(int id) {
    BackgroundLayer *ret = new(std::nothrow) BackgroundLayer();
    if (ret && ret->init(id)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BackgroundLayer::init() {
    if (!LayerColor::initWithColor(Color4B::WHITE)) {
        return false;
    }
    _id = 0;
    _parallax = nullptr;

    return true;
}

BackgroundLayer *BackgroundLayer::create() {
    BackgroundLayer *ret = new(std::nothrow) BackgroundLayer();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


void BackgroundLayer::removeSprites() {
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("background-" + StringUtils::toString(_id) + ".plist");
}


void BackgroundLayer::move(float delta) {
    setPosition(_parallax->getPosition().x - delta, _parallax->getPosition().y);
}

void BackgroundLayer::setPosition(float x, float y) {
    _parallax->setPosition(x, y);
}

Vec3 BackgroundLayer::getPosition3D() const {
    return _parallax->getPosition3D();
}
