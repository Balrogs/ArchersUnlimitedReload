//
// Created by igor on 14.09.16.
//

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

    const auto transform = node1->getNodeToWorldTransform();
    transform.transformPoint(&localPoint);
    node2->getWorldToNodeTransform().transformPoint(&localPoint);
    return cocos2d::Vec2(localPoint.x, localPoint.y);
}

cocos2d::Vec2 Variables::translatePoint(cocos2d::Vec3 localPoint, cocos2d::Node *node) {
    node->getNodeToWorldTransform().transformPoint(&localPoint);
    return cocos2d::Vec2(localPoint.x, localPoint.y);
}
