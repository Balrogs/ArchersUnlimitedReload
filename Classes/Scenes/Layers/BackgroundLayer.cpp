//
// Created by igor on 13.10.16.
//

#include <GameEngine/Objects/Environment/Ground.h>
#include <Scenes/PlayLayers/Battle.h>
#include <GameEngine/Global/Variables.h>
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

    this->addChild(hills2, 1, Point(0.2, 0.2), Point::ZERO);

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
    this->addChild(moon, 2, Point(0, 0), Point(visibleSize.width / 2 + 2 * moon->getBoundingBox().size.width, y));

    //fences

    auto f = Sprite::createWithSpriteFrameName("fence1.png");
    auto fenceWidth = f->getBoundingBox().size.width / 2;
    auto fenceYpos = 300.f;
    unsigned int fencesQuantity = 30;
    for (unsigned int i = 0; i < fencesQuantity; i++) {
        auto random = RandomHelper::random_int(1, 3);
        CCLOG("%d", random);
        auto fence = Sprite::createWithSpriteFrameName("fence" + StringUtils::toString(random) + ".png");
        if(fence == nullptr){
            continue;
        }
        fence->setAnchorPoint(Point::ZERO);
        fence->setTag(1000 + i);
        fence->setScale(0.4f);
        if (random == 1) {
            this->addChild(fence, 1, Point(0.2, 0.2),
                           Point((-visibleSize.width / 2) + (i + 1) * fenceWidth, fenceYpos));
        } else {
            this->addChild(fence, 1, Point(0.2, 0.2),
                           Point((-visibleSize.width / 2) + (i + 1) * fenceWidth, fenceYpos - 20.f));
        }

    }

    //trees

    unsigned int treesQuantity = 25;
    for (unsigned int i = 0; i < treesQuantity; i++) {
        auto tree = Sprite::createWithSpriteFrameName(
                "tree" + StringUtils::toString(RandomHelper::random_int(1, 2)) + ".png");
        tree->setAnchorPoint(Point::ZERO);
        int randomZorder = RandomHelper::random_int(2, 3);
        tree->setScale(2 * randomZorder / 10.f);
        tree->setTag(2000 + i);

        CCLOG("randomZorder %d Ratio %f", randomZorder, 0.1f + randomZorder / 7.f);
        this->addChild(tree, randomZorder, Point(0.1f + randomZorder / 7.f, 0.1f + randomZorder / 7.f),
                       Point((-visibleSize.width / 2) + (i + 1) * RandomHelper::random_int(100, 250),
                             RandomHelper::random_real(fenceYpos - 150, fenceYpos - 20)));
    }

    //graves

    unsigned int gravesQuantity = 15;
    for(unsigned int i = 0; i < gravesQuantity; i++)
    {
        auto grave = Sprite::createWithSpriteFrameName("grave" + StringUtils::toString(RandomHelper::random_int(1, 3)) +".png");
        grave->setAnchorPoint(Point::ZERO);
        grave->setTag(3000 + i);
        this->addChild(grave, 3, Point(0.4f, 0.4f),
                       Point((-visibleSize.width / 2) + (i + 1) * RandomHelper::random_int(100, 250),
                             RandomHelper::random_real(fenceYpos - 100, fenceYpos - 30)));
    }



    //grass

//    unsigned int grassQuantity = 35;
//    for(unsigned int i = 0; i < grassQuantity; i++)
//    {
//        auto grass = Sprite::createWithSpriteFrameName("grass" + StringUtils::toString(RandomHelper::random_int(1, 2)) +".png");
//        grass->setAnchorPoint(Point::ZERO);
//        grass->setScale(RandomHelper::random_real(0.3f, 1.f));
//        grass->setTag(1000 + i);
//        this->addChild(grass, RandomHelper::random_int(2, 3), Point(0.5, 1),
//                       Point((visibleSize.width / 5) * (i + 1) + RandomHelper::random_int(0, 100),  RandomHelper::random_int(50, 150)));
//    }

}

void InfiniteParallaxNode::createDesert() {

    auto visibleSize = Director::getInstance()->getVisibleSize();

    Ground *mountains = Ground::create(BattleScene::instance->GROUND, visibleSize.width * 10, "mountain4.png");

    this->addChild(mountains, 0, Point(0.1, 0.1), Point(0.f,  visibleSize.height / 2 ));

    Ground *hills3 = Ground::create(BattleScene::instance->GROUND, visibleSize.width * 10, "hills3.png");

    this->addChild(hills3, 0, Point(0.1, 0.1), Point(0.f, 12 * mountains->getPosition().y / 10 - hills3->getHeight()));

    auto sun = Sprite::createWithSpriteFrameName("sun.png");
    auto y = visibleSize.height - 100.f;
    this->addChild(sun, 2, Point(0.01f, 0.01f), Point(visibleSize.width / 2 - 2 * sun->getBoundingBox().size.width, y));

    //trees

    //mountains



}

void InfiniteParallaxNode::createGreenfield() {

    auto visibleSize = Director::getInstance()->getVisibleSize();

    //clouds

    unsigned int cloudsQuantity = 25;
    for (unsigned int i = 0; i < cloudsQuantity; i++) {
        auto cloud = Sprite::createWithSpriteFrameName(
                "cloud" + StringUtils::toString(RandomHelper::random_int(1, 3)) + ".png");
        cloud->setAnchorPoint(Point::ZERO);
        cloud->setTag(1000 + i);
        float randomRatio = RandomHelper::random_int(2, 4) / 10.f;
        this->addChild(cloud, 1, Point(randomRatio, randomRatio),
                       Point((-visibleSize.width / 2) + (i + 1) * RandomHelper::random_int(100, 250),
                             RandomHelper::random_real(visibleSize.height - cloud->getBoundingBox().size.height - 150, visibleSize.height - cloud->getBoundingBox().size.height - 20)));
    }

    //trees

    //grass
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

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto bg = Sprite::createWithSpriteFrameName(Variables::BG3);
    bg->setScale(visibleSize.width / bg->getContentSize().width);
    bg->setPosition(visibleSize.width / 2 , visibleSize.height - bg->getBoundingBox().size.height / 2);

    this->addChild(bg, 1);
    
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
