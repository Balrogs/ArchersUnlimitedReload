//
// Created by igor on 04.10.16.
//
#include <GameEngine/Objects/Brains/Brain.h>
#include <Scenes/PlayLayers/Battle.h>
#include "Producer.h"

USING_NS_CC;
using namespace rapidjson;

LevelElement::LevelElement(float time, int type) {
    _time = time;
    _type = type;
}

Level::Level() {
    id = 1;
    _elements.push_back(new LevelElement(1, 1));
    _elements.push_back(new LevelElement(1.2f, 2));
    _elements.push_back(new LevelElement(1.3f, 3));
    _elements.push_back(new LevelElement(2.f, 4));
}

LevelElement *Level::next() {
    auto element = _elements.at(_elements.size() - 1);
    _elements.pop_back();
    return element;
}

bool Level::hasNext() {
    return _elements.size() > 0;
}


void Producer::startLevel(int id) {
    for (auto level : _levels) {
        if (level->id == id) {
            while (level->hasNext()) {
                auto le = level->next();
                Body *target = nullptr;
                Brain *brain = nullptr;
                DelayTime *pDelayTime = DelayTime::create(le->_time);

                switch (le->_type) {
                    case 0:
                        target = new Stickman(BattleScene::getInstance()->visibleSize.width + 150.f,
                                              BattleScene::getInstance()->GROUND,
                                              0.3f,
                                              100.f);
                        brain = new PassiveBrain(target);
                        break;
                    case 1:
                        target = new Stickman(BattleScene::getInstance()->visibleSize.width + 150.f,
                                              BattleScene::getInstance()->GROUND,
                                              0.3f,
                                              100.f);
                        brain = new Brain(target);
                        break;
                    case 2:
                        target = new Stickman(BattleScene::getInstance()->visibleSize.width + 150.f,
                                              BattleScene::getInstance()->GROUND,
                                              0.25f,
                                              50.f);
                        brain = new FastBrain(target);
                        break;
                    case 3:
                        target = new Stickman(BattleScene::getInstance()->visibleSize.width + 150.f,
                                              BattleScene::getInstance()->GROUND,
                                              0.5f,
                                              300.f);
                        brain = new StrengthBrain(target);
                        break;
                    case 4:
//                        target = new Hero(BattleScene::getInstance()->visibleSize.width + 150.f, BattleScene::getInstance()->GROUND, Bot::create());
//                        brain = new HeroBrain(target, 0.f);
                        break;
                    default:
                        break;
                }
                if(target != nullptr && brain != nullptr){
                    CallFunc *addtarg =  CallFunc::create(CC_CALLBACK_0(Producer::_addTarget, this, target, brain));
                    BattleScene::getInstance()->runAction(Sequence::create(pDelayTime, addtarg, NULL));
                }
            }
            break;
        }
    }
}

Producer::Producer(const char *path) {
//    Document document;
//    document.Parse(path);
//    if (Producer *prod = dynamic_cast<Producer *>(document.Get())) {
//        this = prod;
//    } else
//        this = nullptr;

    _levels.push_back(new Level());
}

void Producer::_addTarget(Body *target, Brain *brain) {
    _battleScene->addTarget(target);
    _battleScene->addBrain(brain);
}

