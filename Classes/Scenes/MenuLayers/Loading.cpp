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

    // TODO add animation of load

    return true;
}