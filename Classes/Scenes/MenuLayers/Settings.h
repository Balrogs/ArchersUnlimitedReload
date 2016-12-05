//
// Created by igor on 13.10.16.
//

#ifndef ARCUNLIM_SETTINGS_H
#define ARCUNLIM_SETTINGS_H

#include "cocos2d.h"

class Settings : public cocos2d::Layer {
public:
    CREATE_FUNC(Settings);

    static cocos2d::Scene *createScene();

    Settings();
    ~Settings();

protected:

    void onEnter() override;

    void onQuit();
};


#endif //ARCUNLIM_SETTINGS_H
