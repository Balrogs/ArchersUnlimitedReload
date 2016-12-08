//
// Created by igor on 02.11.16.
//

#ifndef ARCHERSUNLIMITED_GROUND_H
#define ARCHERSUNLIMITED_GROUND_H

#include "cocos2d.h"

class Ground : public cocos2d::Node {

public:

    bool init(float y_pos, float width, std::string path);

    static Ground* create(float y_pos, float width, std::string path);
};

class Clouds : public cocos2d::Node {

public:

    bool init(float width, std::string path);

    static Clouds* create(float width, std::string path);
};


#endif //ARCHERSUNLIMITED_GROUND_H
