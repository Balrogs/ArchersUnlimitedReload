//
// Created by igor on 13.10.16.
//

#ifndef ARCUNLIM_SETTINGS_H
#define ARCUNLIM_SETTINGS_H

#include "cocos2d.h"

class Settings : public cocos2d::Layer {
public:
    CREATE_FUNC(Settings);

    bool init();

protected:

    void onEnter() override;

    void onQuit();
};


#endif //ARCUNLIM_SETTINGS_H
