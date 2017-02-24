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
    const auto dragonBonesData = factory.loadDragonBonesData("ArcUnlimArmature.json");
    factory.loadTextureAtlasData("texture.json");

    auto loadingArmature = factory.buildArmature("Loading");
    auto display = (dragonBones::CCArmatureDisplay *) loadingArmature->getDisplay();

    dragonBones::WorldClock::clock.add(loadingArmature);

    display->setPosition(visibleSize.width /2 , visibleSize.height /2);

    this->addChild(display);

    loadingArmature->getAnimation().fadeIn("loading");

    return true;
}