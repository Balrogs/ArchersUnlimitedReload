//
// Created by igor on 04.10.16.
//

#ifndef ARCUNLIM_PRODUCER_H
#define ARCUNLIM_PRODUCER_H

#include <GameEngine/Objects/Brains/Brain.h>
#include "cocos2d.h"

class LevelElement {
public:
    LevelElement(float time, int type);

public:
    float _time;
    int _type;
};

class Level {
public:
    int id;

    Level();

    LevelElement *next();

    bool hasNext();

private:
    std::vector<LevelElement *> _elements;
};

class Producer {
public:
    Producer(const char *path);

    void startLevel(int id);

private:
    void _addTarget(Body *target, Brain *brain);

    std::vector<Level *> _levels;
};


#endif //ARCUNLIM_PRODUCER_H
