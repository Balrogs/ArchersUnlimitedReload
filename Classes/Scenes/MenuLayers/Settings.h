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

    void _reloadButtons();

    cocos2d::ui::Button *_musicButton;
    cocos2d::ui::Button *_effectsButton;

    bool _musicState;

    bool _effectsState;

    void onEnter() override;

    void onQuit();

    cocos2d::Sprite *_bg;
};


#endif //ARCUNLIM_SETTINGS_H
