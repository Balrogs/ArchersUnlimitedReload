#include <GameEngine/Global/Variables.h>
#include <Scenes/MenuLayers/MainMenu.h>
#include "MainClip.h"

USING_NS_CC;

MainClip *MainClip::create() {
    MainClip *ret = new(std::nothrow) MainClip();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MainClip::init() {
    if (!Sprite::initWithSpriteFrameName(Variables::MAIN_BACKGROUND))
        return false;
    _totalWay = this->getContentSize().width;
    _endPos = this->getContentSize().width + 50.f;
    _starPos =- 50.f;

    auto cloud1 = Sprite::createWithSpriteFrameName(Variables::CLOUD_1);
    cloud1->setPosition(Vec2(
            this->getContentSize().width / 2,
            this->getContentSize().height - 0.75f * cloud1->getBoundingBox().size.height
    ));
    this->addChild(cloud1, 3);
    cloud1->runAction(MoveTo::create(
            60 * (_totalWay - cloud1->getPosition().x) / _totalWay * 1.6f,
            Vec2(_endPos, cloud1->getPosition().y))
    );
    _objects.insert(std::pair<Node*, float> (cloud1, 1.6f));

    auto cloud2 = Sprite::createWithSpriteFrameName(Variables::CLOUD_2);
    cloud2->setPosition(Vec2(
            this->getContentSize().width / 4,
            this->getContentSize().height- 0.4f * cloud2->getBoundingBox().size.height
    ));
    this->addChild(cloud2, 5);
    cloud2->runAction(MoveTo::create(
            60 * (_totalWay - cloud2->getPosition().x) / _totalWay * 2.f,
            Vec2(_endPos, cloud2->getPosition().y))
    );
    _objects.insert(std::pair<Node*, float> (cloud2, 2.f));

    auto cloud3 = Sprite::createWithSpriteFrameName(Variables::CLOUD_3);
    cloud3->setPosition(Vec2(
            0,
            this->getContentSize().height - cloud3->getBoundingBox().size.height
    ));
    this->addChild(cloud3, 2);
    cloud3->runAction(MoveTo::create(
            60 * (_totalWay - cloud3->getPosition().x) / _totalWay * 1.4f,
            Vec2(_endPos, cloud3->getPosition().y))
    );
    _objects.insert(std::pair<Node*, float> (cloud3, 1.4f));

    auto cloud4 = Sprite::createWithSpriteFrameName(Variables::CLOUD_2);
    cloud4->setPosition(Vec2(
            this->getContentSize().width * 0.75f,
            this->getContentSize().height - 0.75f * cloud4->getBoundingBox().size.height
    ));
    this->addChild(cloud4, 3);
    cloud4->runAction(MoveTo::create(
            60 * (_totalWay - cloud4->getPosition().x) / _totalWay * 1.6f,
            Vec2(_endPos, cloud4->getPosition().y))
    );
    _objects.insert(std::pair<Node*, float> (cloud4, 1.6f));

    auto cloud5 = Sprite::createWithSpriteFrameName(Variables::CLOUD_1);
    cloud5->setPosition(Vec2(
            this->getContentSize().width * 0.6f,
            this->getContentSize().height - 0.6f * cloud5->getBoundingBox().size.height
    ));
    this->addChild(cloud5, 4);
    cloud5->runAction(MoveTo::create(
            60 * (_totalWay - cloud5->getPosition().x) / _totalWay *  1.8f,
            Vec2(_endPos, cloud5->getPosition().y))
    );
    _objects.insert(std::pair<Node*, float> (cloud5, 1.8f));


    this->schedule(SEL_SCHEDULE(&MainClip::update), 1.f);
    return true;
}

void MainClip::update(float dt) {
    Node::update(dt);
    for(auto cloud: _objects){
        if(cloud.first->getPosition().x == _endPos){
            cloud.first->setPositionX(_starPos);
            cloud.first->runAction(MoveTo::create(60 * cloud.second, Vec2(_endPos,   cloud.first->getPosition().y)));
        }
    }
}