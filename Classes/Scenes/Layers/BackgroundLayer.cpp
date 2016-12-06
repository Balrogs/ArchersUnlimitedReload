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

    switch (id) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        default:
            return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    Ground *ground = new Ground(BattleScene::instance->GROUND, visibleSize.width * 3);

    this->addChild(ground, 1, Point(0.5, 1),
                   Point::ZERO);

//    auto tree = Sprite::create("rock.png");
//    auto rock = Sprite::create("rock.png");
//
//    unsigned int rocksQuantity = 7;
//    for(unsigned int i = 0; i < rocksQuantity; i++)
//    {
//        // Create a sprite with rock texture
//        auto rock = Sprite::create("rock.png");
//        rock->setAnchorPoint(Point::ZERO);
//        // Set scale factor as a random value from [0.8, 1.2] interval
//        rock->setScale((float) RandomHelper::random_real(0.6, 0.75));
//        rock->setTag(1000 + i);
//        this->addChild(rock,
//                // Set random z-index from [-10,-6]
//                            RandomHelper::random_int(-10, -6),
//                // Set ration (rocks moves slow)
//                            Point(0.5, 1),
//                // Set position with random component
//                            Point((visibleSize.width / 5) * (i + 1) + RandomHelper::random_int(0, 100),
//                                  100));
//    }
//
//    unsigned int treesQuantity = 35;
//    for(unsigned int i = 0; i < treesQuantity; i++)
//    {
//        auto tree = Sprite::create("tree.png");
//        tree->setAnchorPoint(Point::ZERO);
//        // Parameters for trees varies
//        tree->setScale((float) RandomHelper::random_real(0.5, 0.75));
//        this->addChild(
//                tree,
//                RandomHelper::random_int(-5, -1),
//                Point(0.75, 1),
//                Point(visibleSize.width / (treesQuantity - 5) * (i + 1) + RandomHelper::random_int(25,50),
//                      80));
//    }

    return true;
}

bool BackgroundLayer::init(int id) {
    if (!LayerColor::initWithColor(Color4B::WHITE)) {
        return false;
    }

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

void BackgroundLayer::move(float delta) {
    Point scrollDecrement = Point(delta, 0);
    _parallax->setPosition(_parallax->getPosition() - scrollDecrement);
    _parallax->updatePosition();
}

BackgroundLayer::BackgroundLayer() {

}

BackgroundLayer::~BackgroundLayer() {

}
