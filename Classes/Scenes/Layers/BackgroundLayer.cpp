#include <GameEngine/Objects/Environment/Ground.h>
#include <Scenes/PlayLayers/Battle.h>
#include <GameEngine/Global/Variables.h>
#include <GameEngine/Objects/Environment/MainClip.h>
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
    GROUND = BattleScene::getInstance()->GROUND;
    _size = Size(-visibleSize.width * 5, visibleSize.width * 10);
    Ground *hills2 = Ground::create(GROUND, _size.height, "hills2.png");

    this->addChild(hills2, 2, Point(0.2, 0.2), Point::ZERO);

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

// TODO
void InfiniteParallaxNode::createGraveyard() {

    _ground = DrawNode::create();

    auto from = Vec2(_size.width, 0);
    auto to = Vec2(_size.height, GROUND);

    _ground->drawSolidRect(from, to, Color4F(Color4B(113, 18, 13, 255)));

    _ground->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(from.x, to.y),
                                                           to));
    _ground->getPhysicsBody()->setContactTestBitmask(true);

    this->addChild(_ground, 3, Point(1, 1), Point::ZERO);

    auto visibleSize = Director::getInstance()->getVisibleSize();

    Clouds *cloud2 = Clouds::create(visibleSize.width * 3, "clouds2.png");

    this->addChild(cloud2, 1, Point(0.2, 0.2), Point::ZERO);

    Clouds *cloud1 = Clouds::create(visibleSize.width * 3, "clouds1.png");

    this->addChild(cloud1, 3, Point(0.1, 0.1), Point::ZERO);

    auto moon = Sprite::createWithSpriteFrameName("moon.png");
    auto y = visibleSize.height - 100.f;
    this->addChild(moon, 2, Point(0, 0), Point(visibleSize.width / 2 + 2 * moon->getBoundingBox().size.width, y));



    //grass

    unsigned int grassFirstGeneration = 25;
    auto prev_pos = -visibleSize.width * 2;
    for (unsigned int i = 0; i < grassFirstGeneration; i++) {
        auto grass = Sprite::createWithSpriteFrameName(
                "grass" + StringUtils::toString(RandomHelper::random_int(1, 2)) + ".png");
        grass->setAnchorPoint(Point::ZERO);
        int zOrder = 4;
        float random_el = zOrder / 5.f + 0.2f + RandomHelper::random_real(0.2f, 0.4f);
        grass->setTag(3000 + i);
        grass->setScale(0.5f);
        prev_pos = prev_pos + RandomHelper::random_int(300, 450) + grass->getBoundingBox().size.width;
        this->addChild(grass, zOrder, Point(1.f, 1.f),
                       Point(prev_pos,
                             RandomHelper::random_real(
                                     GROUND - grass->getBoundingBox().size.height / 2,
                                     GROUND)));
    }
}

void InfiniteParallaxNode::createDesert() {

    _ground = DrawNode::create();

    auto from = Vec2(_size.width, 0);
    auto to = Vec2(_size.height, GROUND);

    _ground->drawSolidRect(from, to, Color4F(Color4B(212, 200, 170, 255)));

    _ground->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(from.x, to.y),
                                                           to));
    _ground->getPhysicsBody()->setContactTestBitmask(true);

    this->addChild(_ground, 3, Point(1, 1), Point::ZERO);

    auto visibleSize = Director::getInstance()->getVisibleSize();

    Ground *mountains = Ground::create(BattleScene::getInstance()->GROUND, visibleSize.width * 10, "mountain4.png");

    this->addChild(mountains, 0, Point(0.1, 0.1), Point(0.f, visibleSize.height / 2));


    unsigned int mountainsFirstGeneration = 15;
    float prev_pos = -visibleSize.width * 2;
    for (unsigned int i = 0; i < mountainsFirstGeneration; i++) {
        auto mountain = Sprite::createWithSpriteFrameName(
                "mountain" + StringUtils::toString(RandomHelper::random_int(1, 3)) + ".png");
        mountain->setAnchorPoint(Point::ZERO);
        int zOrder = 0;
        float random_el = RandomHelper::random_real(0.6f, 0.8f);
        mountain->setScale(random_el);
        mountain->setTag(1000 + i);
        prev_pos = prev_pos + RandomHelper::random_int(50, 400) + mountain->getBoundingBox().size.width;
        this->addChild(mountain, zOrder, Point(0.15, 0.15),
                       Point(prev_pos,
                             mountains->getPosition().y));
    }


    Ground *hills3 = Ground::create(BattleScene::getInstance()->GROUND, visibleSize.width * 10, "hills3.png");

    this->addChild(hills3, 0, Point(0.1, 0.1), Point(0.f, 12 * mountains->getPosition().y / 10 - hills3->getHeight()));

    auto sun = Sprite::createWithSpriteFrameName("sun.png");
    auto y = visibleSize.height - 100.f;
    this->addChild(sun, 2, Point(0.01f, 0.01f), Point(visibleSize.width / 2 - 2 * sun->getBoundingBox().size.width, y));

    //trees


    unsigned int treesFirstGeneration = 35;
    prev_pos = -visibleSize.width * 2;
    float y_pos = hills3->getPosition().y + hills3->getHeight() / 2;
    for (unsigned int i = 0; i < treesFirstGeneration; i++) {
        auto tree = Sprite::createWithSpriteFrameName(
                "tree" + StringUtils::toString(RandomHelper::random_int(1, 2)) + ".png");
        tree->setAnchorPoint(Point::ZERO);
        int zOrder = 1;
        float random_el = RandomHelper::random_real(0.2f, 0.4f);
        tree->setScale(random_el);
        tree->setTag(1000 + i);
        prev_pos = prev_pos + RandomHelper::random_int(100, 250) + tree->getBoundingBox().size.width;
        this->addChild(tree, zOrder, Point(0.1f, 0.1f),
                       Point(prev_pos,
                             RandomHelper::random_real(
                                     y_pos - tree->getBoundingBox().size.height,
                                     y_pos + tree->getBoundingBox().size.height)));
    }

    unsigned int treesSecondGeneration = 15;
    prev_pos = -visibleSize.width * 2;
    for (unsigned int i = 0; i < treesSecondGeneration; i++) {
        auto tree = Sprite::createWithSpriteFrameName(
                "tree" + StringUtils::toString(RandomHelper::random_int(1, 2)) + ".png");
        tree->setAnchorPoint(Point::ZERO);
        int zOrder = 2;
        float random_el = RandomHelper::random_real(0.8f, 1.2f);
        tree->setScale(random_el);
        tree->setTag(2000 + i);
        prev_pos = prev_pos + RandomHelper::random_int(300, 700) + tree->getBoundingBox().size.width;
        this->addChild(tree, zOrder, Point(0.2f, 0.2f),
                       Point(prev_pos,
                             RandomHelper::random_real(
                                     GROUND,
                                     GROUND +
                                     tree->getBoundingBox().size.height / 3)));
    }

}

void InfiniteParallaxNode::createGreenfield() {

    _ground = DrawNode::create();

    auto from = Vec2(_size.width, 0);
    auto to = Vec2(_size.height, GROUND);

    _ground->drawSolidRect(from, to, Color4F(Color4B(89, 123, 62, 255)));

    _ground->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(from.x, to.y),
                                                           to));
    _ground->getPhysicsBody()->setContactTestBitmask(true);

    this->addChild(_ground, 3, Point(1, 1), Point::ZERO);

    auto visibleSize = Director::getInstance()->getVisibleSize();

    //clouds

    unsigned int cloudsQuantity = 25;
    for (unsigned int i = 0; i < cloudsQuantity; i++) {
        auto cloud = Sprite::createWithSpriteFrameName(
                "Cloud" + StringUtils::toString(RandomHelper::random_int(1, 3)) + ".png");
        cloud->setAnchorPoint(Point::ZERO);
        cloud->setTag(1000 + i);
        float randomRatio = RandomHelper::random_int(2, 4) / 10.f;
        this->addChild(cloud, 1, Point(randomRatio, randomRatio),
                       Point((-visibleSize.width / 2) + (i + 1) * RandomHelper::random_int(100, 250),
                             RandomHelper::random_real(visibleSize.height - cloud->getBoundingBox().size.height - 150,
                                                       visibleSize.height - cloud->getBoundingBox().size.height - 20)));
    }

    //trees

    unsigned int treesFirstGeneration = 35;
    auto prev_pos = -visibleSize.width * 2;
    for (unsigned int i = 0; i < treesFirstGeneration; i++) {
        auto tree = Sprite::createWithSpriteFrameName(
                "tree" + StringUtils::toString(RandomHelper::random_int(1, 3)) + ".png");
        tree->setAnchorPoint(Point::ZERO);
        int zOrder = 2;
        float random_el = RandomHelper::random_real(0.2f, 0.4f);
        tree->setScale(random_el);
        tree->setTag(1000 + i);
        prev_pos = prev_pos + RandomHelper::random_int(100, 250) + tree->getBoundingBox().size.width;
        this->addChild(tree, zOrder, Point(0.1f, 0.1f),
                       Point(prev_pos,
                             RandomHelper::random_real(
                                     GROUND,
                                     GROUND +
                                     tree->getBoundingBox().size.height)));
    }

    unsigned int treesSecondGeneration = 15;
    prev_pos = -visibleSize.width * 2;
    for (unsigned int i = 0; i < treesSecondGeneration; i++) {
        auto tree = Sprite::createWithSpriteFrameName(
                "tree" + StringUtils::toString(RandomHelper::random_int(1, 3)) + ".png");
        tree->setAnchorPoint(Point::ZERO);
        int zOrder = 2;
        float random_el = RandomHelper::random_real(0.8f, 1.2f);
        tree->setScale(random_el);
        tree->setTag(2000 + i);
        prev_pos = prev_pos + RandomHelper::random_int(300, 500) + tree->getBoundingBox().size.width;
        this->addChild(tree, zOrder, Point(0.2f, 0.2f),
                       Point(prev_pos,
                             RandomHelper::random_real(
                                     GROUND,
                                     GROUND +
                                     tree->getBoundingBox().size.height / 3)));
    }

    //grass

    unsigned int grassFirstGeneration = 25;
    prev_pos = -visibleSize.width * 2;
    for (unsigned int i = 0; i < grassFirstGeneration; i++) {
        auto grass = Sprite::createWithSpriteFrameName(
                "grass" + StringUtils::toString(RandomHelper::random_int(1, 2)) + ".png");
        grass->setAnchorPoint(Point::ZERO);
        int zOrder = 4;
        float random_el = zOrder / 5.f + 0.2f + RandomHelper::random_real(0.2f, 0.4f);
        grass->setScale(0.5f);
        grass->setTag(3000 + i);
        prev_pos = prev_pos + RandomHelper::random_int(300, 450) + grass->getBoundingBox().size.width;
        this->addChild(grass, zOrder, Point(1.f, 1.f),
                       Point(prev_pos,
                             RandomHelper::random_real(
                                     GROUND - grass->getBoundingBox().size.height / 2,
                                     GROUND)));
    }
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

    auto bg = Sprite::createWithSpriteFrameName(Variables::MAIN);
    auto scale = visibleSize.width / bg->getContentSize().width;
    bg->setScale(scale);
    bg->setPosition(visibleSize.width / 2, visibleSize.height - bg->getBoundingBox().size.height / 2);

    this->addChild(bg, 1);

    auto mainClip = MainClip::create();
    mainClip->setScale(scale);
    mainClip->setPosition(
            0.23f * bg->getBoundingBox().size.width,
            bg->getBoundingBox().getMaxY() - 7.8f * mainClip->getBoundingBox().size.height / 2);

    this->addChild(mainClip, 0, "clip");

    _waiting = Sprite::createWithSpriteFrameName(Variables::WAITING_LAYER);
    _waiting->setScale(scale);
    _waiting->setPosition(visibleSize.width / 2, visibleSize.height - _waiting->getBoundingBox().size.height / 2);
    this->addChild(_waiting, 3);

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

void BackgroundLayer::addArrow(Node *arrow) {
    _parallax->addChild(arrow, 10, Point(1, 1), Point::ZERO);
}

void BackgroundLayer::wait(bool p) {
    _waiting->setVisible(p);
}
