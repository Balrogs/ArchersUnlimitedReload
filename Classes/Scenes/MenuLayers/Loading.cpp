#include <GameEngine/Global/Variables.h>
#include "Loading.h"
#include "MainMenu.h"


USING_NS_CC;

bool Loading::init() {
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto black = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(black);

    dragonBones::CCFactory factory;
    factory.loadDragonBonesData("ArcUnlimArmature.json");
    factory.loadTextureAtlasData("texture.json");

    auto loadingArmature = factory.buildArmature("Loading");
    auto display = (dragonBones::CCArmatureDisplay *) loadingArmature->getDisplay();

    display->setPosition(visibleSize.width /2 , visibleSize.height /2);
    //TODO WHAT THE FUCK  IS HERE ???!!!
    loadingArmature->getAnimation().fadeIn("loading");

    dragonBones::WorldClock::clock.add(loadingArmature);
    this->addChild(display);
    return true;
}