//
// Created by igor on 04.10.16.
//

#include "MainMenu.h"
#include "Scenes/PlayLayers/Battle.h"

USING_NS_CC;

Scene *MainMenu::createScene() {
    auto scene = Scene::create();
    MainMenu *layer = MainMenu::create();
    scene->addChild(layer);
    return scene;
}

MainMenu::MainMenu() {
    auto item1 = MenuItemFont::create("Waves", CC_CALLBACK_0(MainMenu::onPushScene, this, 0));
    auto item2 = MenuItemFont::create("Apple", CC_CALLBACK_0(MainMenu::onPushScene, this, 1));
    auto item3 = MenuItemFont::create("Duel", CC_CALLBACK_0(MainMenu::onPushScene, this, 2));
    auto item4 = MenuItemFont::create("Quit", CC_CALLBACK_1(MainMenu::onQuit, this));

    auto menu = Menu::create(item1, item2, item3, item4, nullptr);
    menu->alignItemsVertically();

    this->addChild(menu);
}

MainMenu::~MainMenu() {

}

void MainMenu::onEnter() {
    Layer::onEnter();
}

void MainMenu::onEnterTransitionDidFinish() {
    Layer::onEnterTransitionDidFinish();
}

void MainMenu::onPushScene(int id) {
    auto scene = BattleScene::createScene(id);
    Director::getInstance()->pushScene(scene);
}

void MainMenu::onQuit(cocos2d::Ref *sender) {
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

}