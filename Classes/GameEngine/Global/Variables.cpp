#include <Scenes/PlayLayers/Battle.h>
#include "Variables.h"

Resolution Variables::_resolution = Resolution::HD;

const std::string  Variables::HERO_IDLE_ANIMATION = "idle_1";
const std::string  Variables::SHOT_ANIMATION = "shot";
const std::string  Variables::AIM_ANIMATION = "aim";
const std::string  Variables::AIM_IDLE_ANIMATION = "idle";

const std::string Variables::STICKMAN_SETUP_ANIMATION = "setup";
const std::string Variables::STICKMAN_IDLE_ANIMATION = "idle_1";
const std::string Variables::STICKMAN_RUN_ANIMATION = "run_1";
const std::string Variables::STICKMAN_WALK_ANIMATION = "walk";
const std::string Variables::STICKMAN_SIT_ANIMATION = "sit";
const std::string Variables::STICKMAN_UP_ANIMATION = "up";
const std::string Variables::STICKMAN_EQUIP_IDLE_ANIMATION = "equipment1";

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

const int Variables::RESET_INTERVAL = 10 * 1000;
const int Variables::MAX_TRY_COUNT = 5;


const std::string Variables::FONT_NAME = "FSEX300.ttf";
const cocos2d::Color3B Variables::FONT_COLOR = cocos2d::Color3B::BLACK;

const cocos2d::Color4F Variables::STRING_COLOR = cocos2d::Color4F::GRAY;

const std::string Variables::YES_BUTTON_PATH = "yes.png";
const std::string Variables::YES_DISABLED_PATH = "yes_disabled.png";
const std::string Variables::YES_PRESSED_BUTTON_PATH = "yes_pressed.png";

const std::string Variables::NO_BUTTON_PATH = "no.png";
const std::string Variables::NO_PRESSED_BUTTON_PATH = "no_pressed.png";

const std::string Variables::BUTTON_PATH = "ok.png";
const std::string Variables::PRESSED_BUTTON_PATH = "ok_pressed.png";

const std::string Variables::CLOSE_BUTTON_PATH = "cancel.png";
const std::string Variables::CLOSE_PRESSED_BUTTON_PATH = "cancel_pressed.png";

const std::string Variables::BACK_PRESSED_BUTTON_PATH = "back_pressed.png";
const std::string Variables::BACK_BUTTON_PATH = "back.png";

const std::string Variables::CANCEL_BUTTON = "cancel.png";
const std::string Variables::CANCEL_PRESSED_BUTTON = "cancel_pressed.png";

const std::string Variables::FIND_BUTTON = "search.png";
const std::string Variables::FIND_PRESSED_BUTTON = "search_pressed.png";

const std::string Variables::MORE_INFO_BUTTON = "moreinfo.png";
const std::string Variables::MORE_INFO_PRESSED_BUTTON = "moreinfo_pressed.png";

const std::string Variables::DROP_LIST_BUTTON = "drop_list.png";
const std::string Variables::DROP_LIST_PRESSED_BUTTON = "drop_list_pressed.png";

const std::string Variables::BAR = "grey_button06.png";

const std::string Variables::DISABLED_BUTTON = "button_disabled.png";


const std::string Variables::MOVE_BUTTON = "move_arrow.png";
const std::string Variables::MOVE_PRESSED_BUTTON = "move_arrow_pressed.png";

const std::string Variables::PAUSE_BUTTON = "pause.png";
const std::string Variables::PAUSE_PRESSED_BUTTON = "pause_pressed.png";

const std::string Variables::SETTINGS_BUTTON = "settings.png";
const std::string Variables::SETTINGS_PRESSED_BUTTON = "settings_pressed.png";


const std::string Variables::MUSIC_ON_BUTTON = "music_on.png";
const std::string Variables::MUSIC_OFF_BUTTON = "music_off.png";

const std::string Variables::EFFECTS_ON_BUTTON = "soundeffects_on.png";
const std::string Variables::EFFECTS_OFF_BUTTON = "soundeffects_off.png";


const std::string Variables::REMOVE_ADS_BUTTON = "removeAds.png";
const std::string Variables::REMOVE_ADS_PRESSED_BUTTON = "removeAds_pressed.png";


const std::string Variables::CUSTOMIZE_BUTTON = "customize.png";


const std::string Variables::COIN_BAR = "coin_bar.png";
const std::string Variables::COIN = "coin.png";

const std::string Variables::VIDEO_BUTTON = "video.png";

const std::string Variables::EVENT_BUTTON = "events.png";
const std::string Variables::EVENT_PRESSED_BUTTON = "events_pressed.png";

const std::string Variables::STAND = "stand.png";

const std::string Variables::YELLOW_BUTTON = "yellow_normal.png";
const std::string Variables::YELLOW_PRESSED_BUTTON = "yellow_pressed.png";

const std::string Variables::RED_BUTTON = "red_normal.png";
const std::string Variables::RED_PRESSED_BUTTON = "red_pressed.png";


const std::string Variables::BLUE_BUTTON = "blue_normal.png";
const std::string Variables::BLUE_PRESSED_BUTTON = "blue_pressed.png";


const std::string Variables::BLUE_EMPTY_BUTTON = "blue_empty.png";
const std::string Variables::BLUE_EMPTY_PRESSED_BUTTON = "blue_empty_pressed.png";

const std::string Variables::GREEN_BUTTON = "green_normal.png";
const std::string Variables::GREEN_PRESSED_BUTTON = "green_pressed.png";

const std::string Variables::AGAIN_BUTTON = "again.png";
const std::string Variables::AGAIN_PRESSED_BUTTON = "again_pressed.png";

const std::string Variables::CONTINUE_BUTTON = "continue.png";
const std::string Variables::CONTINUE_PRESSED_BUTTON = "continue_pressed.png";

const std::string Variables::BG1 = "bg1.png";
const std::string Variables::BG2 = "bg2.png";
const std::string Variables::BG3 = "bg3.png";

const std::string Variables::WAITING_LAYER = "main-waiting.png";

const std::string Variables::CHEST_BUTTON = "chest.png";
const std::string Variables::CHEST_PRESSED_BUTTON = "chest_pressed.png";

const std::string Variables::GRAY_BUTTON = "grey_button06.png";


const std::string Variables::RATE_ICON = "rate.png";

//Lobby ui

const std::string Variables::BACK_1 = "back1.png";
const std::string Variables::BACK_2 = "back2.png";
const std::string Variables::BACK_3 = "back3.png";
const std::string Variables::BUTTON_1 = "button1.png";
const std::string Variables::BUTTON_2 = "button2.png";
const std::string Variables::BUTTON_3 = "button3.png";

//Main background

const std::string Variables::MAIN_BACKGROUND = "background.png";
const std::string Variables::CLOUD_1 = "cloud1.png";
const std::string Variables::CLOUD_2 = "cloud2.png";
const std::string Variables::CLOUD_3 = "cloud3.png";

const std::string Variables::UI_ARROW = "selector_arrow.png";
const std::string Variables::HAND = "hand.png";
const std::string Variables::WHEEL = "wheel.png";


void Variables::setResolution(Resolution resolution) {
    Variables::_resolution = resolution;
}

float Variables::FONT_SIZE() {
    switch(Variables::_resolution){
        case SD:
            return 20.f;
        case HD:
            return 42.f;
        case IPAD:
            return 68.f;
    }
}

float Variables::FONT_SIZE_2() {
    switch(Variables::_resolution){
        case SD:
            return 30.f;
        case HD:
            return 60.f;
        case IPAD:
            return 80.f;
    }
}

float Variables::H_FONT_SIZE() {
    switch(Variables::_resolution){
        case SD:
            return 42.f;
        case HD:
            return 72.f;
        case IPAD:
            return 134.f;
    }
}

long Variables::getCurrentTime() {
    std::time_t now= std::time(0);
    std::tm* now_tm= std::gmtime(&now);
    return mktime(now_tm) * 1000;
}

float Variables::getBoneFactor(std::string name) {
    if(name == "head"){
        return 1.5f;
    } else if(name == "body") {
        return 1.f;
    } else if(name == "left arm" || name == "right arm"){
        return 0.75f;
    } else if(name == "left leg" || name == "right leg"){
        return 0.75f;
    }  else if(name == "left a" || name == "right a"){
        return .5f;
    } else if(name == "left l" || name == "right l"){
        return .5f;
    } else {
        return 0.f;
    }
}

int Variables::wheelCost() {
    return 500;
}

int Variables::continueCost() {
    return 1000;
}





