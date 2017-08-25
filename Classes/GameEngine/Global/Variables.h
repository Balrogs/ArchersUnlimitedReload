//
// Created by igor on 14.09.16.
//

#ifndef ARCUNLIM_VARIABLES_H
#define ARCUNLIM_VARIABLES_H

#include <dragonBones/cocos2dx/CCArmatureDisplay.h>
#include "cocos2d.h"

enum Resolution {
    SD,
    HD,
    IPAD
};

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
    static const std::string STICKMAN_EQUIP_IDLE_ANIMATION;

    static cocos2d::Vec2 translatePoint(cocos2d::Vec3 localPoint, cocos2d::Node *node1,  cocos2d::Node *node2);
    static cocos2d::Vec2 translatePoint(cocos2d::Vec3 localPoint, cocos2d::Node *node);

    static long getCurrentTime();

    static void setResolution(Resolution resolution);

    static const int RESET_INTERVAL;
    static const int MAX_TRY_COUNT;

    static const std::string FONT_NAME;
    static float FONT_SIZE();
    static float FONT_SIZE_2();
    static float H_FONT_SIZE();
    static const cocos2d::Color3B FONT_COLOR;


    static const cocos2d::Color4F STRING_COLOR;

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

    static const std::string EVENT_BUTTON;
    static const std::string EVENT_PRESSED_BUTTON;


    static const std::string STAND;

    static const std::string YELLOW_BUTTON;
    static const std::string YELLOW_PRESSED_BUTTON;

    static const std::string RED_BUTTON;
    static const std::string RED_PRESSED_BUTTON;

    static const std::string BLUE_BUTTON;
    static const std::string BLUE_PRESSED_BUTTON;

    static const std::string BLUE_EMPTY_BUTTON;
    static const std::string BLUE_EMPTY_PRESSED_BUTTON;

    static const std::string GREEN_BUTTON;
    static const std::string GREEN_PRESSED_BUTTON;

    static const std::string AGAIN_BUTTON;
    static const std::string AGAIN_PRESSED_BUTTON;

    static const std::string CONTINUE_BUTTON;
    static const std::string CONTINUE_PRESSED_BUTTON;

    static const std::string BG1;
    static const std::string BG2;
    static const std::string BG3;

    static const std::string WAITING_LAYER;

    static const std::string CHEST_BUTTON;
    static const std::string CHEST_PRESSED_BUTTON;

    static const std::string GRAY_BUTTON;

    static const std::string RATE_ICON;
    static const std::string SINGLE_ICON;
    static const std::string TWO_PLAYERS_ICON;
    static const std::string MULTIPLAYER_ICON;
    static const std::string APPLE_ICON;
    static const std::string BLOCK_ICON;

    static const std::string YES_DISABLED_PATH;
    static const std::string DISABLED_BUTTON;

    static const std::string BACK_1;
    static const std::string BACK_2;
    static const std::string BACK_3;
    static const std::string BUTTON_1;
    static const std::string BUTTON_2;
    static const std::string BUTTON_3;


    static const std::string MAIN_BACKGROUND;
    static const std::string CLOUD_1;
    static const std::string CLOUD_2;
    static const std::string CLOUD_3;


    static const std::string UI_ARROW;
    static const std::string HAND;
    static const std::string WHEEL;

    static float getBoneFactor(std::string name);

private:
    static Resolution _resolution;

};


#endif //ARCUNLIM_VARIABLES_H
