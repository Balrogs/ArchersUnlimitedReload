//
// Created by igor on 14.09.16.
//

#include <Scenes/PlayLayers/Battle.h>
#include "Variables.h"

const std::string  Variables::HERO_IDLE_ANIMATION = "idle_1";
const std::string  Variables::SHOT_ANIMATION = "shot";
const std::string  Variables::AIM_ANIMATION = "aim";
const std::string  Variables::AIM_IDLE_ANIMATION = "idle";

const std::string Variables::STICKMAN_SETUP_ANIMATION = "idle";
const std::string Variables::STICKMAN_IDLE_ANIMATION = "idle_1";
const std::string Variables::STICKMAN_RUN_ANIMATION = "run_1";
const std::string Variables::STICKMAN_WALK_ANIMATION = "walk";
const std::string Variables::STICKMAN_SIT_ANIMATION = "sit";
const std::string Variables::STICKMAN_UP_ANIMATION = "up";

cocos2d::Vec2 Variables::translatePoint(cocos2d::Vec3 localPoint, cocos2d::Node *node1,
                                        cocos2d::Node *node2) {

    node1->getNodeToWorldTransform().transformPoint(&localPoint);
    node2->getWorldToNodeTransform().transformPoint(&localPoint);
    return cocos2d::Vec2(localPoint.x, localPoint.y);
}

cocos2d::Vec2 Variables::translatePoint(cocos2d::Vec3 localPoint, cocos2d::Node *node) {
    node->getNodeToWorldTransform().transformPoint(&localPoint);
    return cocos2d::Vec2(localPoint.x, localPoint.y);
}

const std::string Variables::FONT_NAME = "Mechanical.ttf";
const float Variables::FONT_SIZE = 25.f;
const float Variables::H_FONT_SIZE = 32.f;
const cocos2d::Color3B Variables::FONT_COLOR = cocos2d::Color3B::BLACK;

const std::string Variables::YES_BUTTON_PATH = "yes.png";
const std::string Variables::YES_PRESSED_BUTTON_PATH = "yes_pressed.png";

const std::string Variables::NO_BUTTON_PATH = "no.png";
const std::string Variables::NO_PRESSED_BUTTON_PATH = "no_pressed.png";

const std::string Variables::BUTTON_PATH = "ok.png";
const std::string Variables::PRESSED_BUTTON_PATH = "ok_pressed.png";

const std::string Variables::CLOSE_BUTTON_PATH = "close.png";
const std::string Variables::CLOSE_PRESSED_BUTTON_PATH = "close_pressed.png";

const std::string Variables::BACK_PRESSED_BUTTON_PATH = "back_button_pressed.png";
const std::string Variables::BACK_BUTTON_PATH = "back_button.png";

const std::string Variables::CANCEL_BUTTON = "cancel.png";
const std::string Variables::CANCEL_PRESSED_BUTTON = "cancel_pressed.png";

const std::string Variables::FIND_BUTTON = "search.png";
const std::string Variables::FIND_PRESSED_BUTTON = "search_pressed.png";

const std::string Variables::MORE_INFO_BUTTON = "drop_list.png";
const std::string Variables::MORE_INFO_PRESSED_BUTTON = "drop_list_pressed.png";

const std::string Variables::BAR = "grey_button06.png";


