//
// Created by igor on 13.10.16.
//

#include "Settings.h"
USING_NS_CC;

cocos2d::Scene *Settings::createScene() {
    auto scene = Scene::create();
    Settings *layer = Settings::create();
    scene->addChild(layer);
    return scene;
}

Settings::Settings() {

}

Settings::~Settings() {

}

void Settings::onEnter() {
  //  Layer::onEnter();
}

void Settings::onQuit() {
    Director::getInstance()->popScene();
}
