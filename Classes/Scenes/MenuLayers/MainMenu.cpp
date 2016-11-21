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
   // auto item1 = MenuItemFont::create("Waves", CC_CALLBACK_0(MainMenu::onPushScene, this, 0));
    auto item2 = MenuItemFont::create("Apple", CC_CALLBACK_0(MainMenu::onPushScene, this, 1));
    auto item3 = MenuItemFont::create("Duel with bot", CC_CALLBACK_0(MainMenu::onPushScene, this, 2));
    auto item4 = MenuItemFont::create("Duel 2P", CC_CALLBACK_0(MainMenu::onPushScene, this, 3));
    auto item5 = MenuItemFont::create("Duel Multiplayer", CC_CALLBACK_0(MainMenu::onChangeLayer, this, 4));
    auto item6 = MenuItemFont::create("Quit", CC_CALLBACK_1(MainMenu::onQuit, this));

    auto menu = Menu::create(item2, item3, item4, item5, item6, nullptr);
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

void MainMenu::onChangeLayer(int id) {
    this->getParent()->addChild(new MultiplayerMainMenu());
    this->removeFromParent();
}

void MainMenu::onQuit(cocos2d::Ref *sender) {
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

}

MultiplayerMainMenu::MultiplayerMainMenu() {
    // auto item1 = MenuItemFont::create("Waves", CC_CALLBACK_0(MainMenu::onPushScene, this, 0));
    auto item2 = MenuItemFont::create("Login", CC_CALLBACK_0(MultiplayerMainMenu::onPushScene, this, 1));
    auto item4 = MenuItemFont::create("Register", CC_CALLBACK_0(MultiplayerMainMenu::onPushScene, this, 2));
    auto item6 = MenuItemFont::create("Back", CC_CALLBACK_1(MultiplayerMainMenu::onQuit, this));

    auto menu = Menu::create(item2, item4, item6, nullptr);
    menu->alignItemsVertically();

    this->addChild(menu);
}

MultiplayerMainMenu::~MultiplayerMainMenu() {

}

void MultiplayerMainMenu::onEnter() {
    Layer::onEnter();
}

void MultiplayerMainMenu::onEnterTransitionDidFinish() {
    Node::onEnterTransitionDidFinish();
}

void MultiplayerMainMenu::onPushScene(int id) {
}

void MultiplayerMainMenu::onQuit(cocos2d::Ref *sender) {
    MainMenu *layer = MainMenu::create();
    this->getParent()->addChild(layer);
    this->removeFromParent();
}
