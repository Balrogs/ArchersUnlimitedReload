//
// Created by igor on 14.09.16.
//

#ifndef ARCUNLIM_VARIABLES_H
#define ARCUNLIM_VARIABLES_H

#include <dragonBones/cocos2dx/CCArmatureDisplay.h>
#include "cocos2d.h"

class Variables {

public:
    static const std::string HERO_IDLE_ANIMATION;
    static const std::string SHOT_ANIMATION;
    static const std::string AIM_ANIMATION;
    static const std::string AIM_IDLE_ANIMATION;

    static const std::string STICKMAN_IDLE_ANIMATION;
    static const std::string STICKMAN_SETUP_ANIMATION;
    static const std::string STICKMAN_RUN_ANIMATION;
    static const std::string STICKMAN_WALK_ANIMATION;
    static const std::string STICKMAN_UP_ANIMATION;
    static const std::string STICKMAN_SIT_ANIMATION;

    static cocos2d::Vec2 translatePoint(cocos2d::Vec3 localPoint, cocos2d::Node *node1,  cocos2d::Node *node2);
    static cocos2d::Vec2 translatePoint(cocos2d::Vec3 localPoint, cocos2d::Node *node);


    static const std::string FONT_NAME;
    static const float FONT_SIZE;
    static const float H_FONT_SIZE;
    static const cocos2d::Color3B FONT_COLOR;

    static const std::string YES_BUTTON_PATH;
    static const std::string NO_BUTTON_PATH;

    static const std::string BUTTON_PATH;
    static const std::string CLOSE_BUTTON_PATH;
    static const std::string YES_PRESSED_BUTTON_PATH;
    static const std::string NO_PRESSED_BUTTON_PATH;
    static const std::string PRESSED_BUTTON_PATH;
    static const std::string CLOSE_PRESSED_BUTTON_PATH;
    static const std::string BACK_PRESSED_BUTTON_PATH;
    static const std::string BACK_BUTTON_PATH;
    static const std::string CANCEL_BUTTON;
    static const std::string CANCEL_PRESSED_BUTTON;
    static const std::string FIND_BUTTON;
    static const std::string FIND_PRESSED_BUTTON;
    static const std::string MORE_INFO_BUTTON;
    static const std::string MORE_INFO_PRESSED_BUTTON;

    static const std::string DROP_LIST_BUTTON;
    static const std::string DROP_LIST_PRESSED_BUTTON;

    static const std::string BAR;

    static const std::string MOVE_BUTTON;
    static const std::string MOVE_PRESSED_BUTTON;
    static const std::string PAUSE_BUTTON;
    static const std::string PAUSE_PRESSED_BUTTON;
    static const std::string SETTINGS_BUTTON;
    static const std::string SETTINGS_PRESSED_BUTTON;

    static const std::string MUSIC_ON_BUTTON;
    static const std::string MUSIC_OFF_BUTTON;

    static const std::string EFFECTS_ON_BUTTON;
    static const std::string EFFECTS_OFF_BUTTON;

    static const std::string REMOVE_ADS_BUTTON;
    static const std::string REMOVE_ADS_PRESSED_BUTTON;

    static const std::string CUSTOMIZE_BUTTON;

    static const std::string COIN_BAR;
    static const std::string COIN;

    static const std::string VIDEO_BUTTON;

    static const std::string STAND;

    static const std::string YELLOW_BUTTON;
    static const std::string YELLOW_PRESSED_BUTTON;

    static const std::string RED_BUTTON;
    static const std::string RED_PRESSED_BUTTON;

    static const std::string BLUE_BUTTON;
    static const std::string BLUE_PRESSED_BUTTON;

    static const std::string GREEN_BUTTON;
    static const std::string GREEN_PRESSED_BUTTON;

    static const std::string BG1;
    static const std::string BG2;

    static const std::string CHEST_BUTTON;
    static const std::string CHEST_PRESSED_BUTTON;
};


#endif //ARCUNLIM_VARIABLES_H
