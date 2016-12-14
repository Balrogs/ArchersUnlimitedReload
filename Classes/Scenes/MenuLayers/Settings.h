//
// Created by igor on 13.10.16.
//

#ifndef ARCUNLIM_SETTINGS_H
#define ARCUNLIM_SETTINGS_H

#include <ui/UIScrollView.h>
#include "cocos2d.h"

class Settings : public cocos2d::Layer {
public:
    CREATE_FUNC(Settings);

    bool init();

protected:
    void _reloadButtons();

    cocos2d::ui::Button *_musicButton;
    cocos2d::ui::Button *_effectsButton;
    cocos2d::ui::Button *_languageBox;
    cocos2d::ui::ScrollView *_scrollView;
    std::string _language;

    bool _musicState;

    bool _effectsState;

    void onEnter() override;

    void onQuit();

    cocos2d::Sprite *_bg;

    void _showScrollView();

};


#endif //ARCUNLIM_SETTINGS_H
