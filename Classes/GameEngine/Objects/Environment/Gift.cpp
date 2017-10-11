#include "Gift.h"
#include <Scenes/MenuLayers/Main/MainMenu.h>
#include <GameEngine/Global/Variables.h>


USING_NS_CC;

bool Gift::init() {
    if (!Node::init()) {
        return false;
    }

    _box = Sprite::createWithSpriteFrameName("gift.png");
    _isBroken = false;
    _isEnabled = false;

    _box->setPosition(Vec2(300, -300));

    this->addChild(_box, 1);

    return true;
}

void Gift::breakBox() {
    if(_isBroken || !_isEnabled){
        return;
    }
    _isBroken = true;
    // TODO show random gift with animation
    // send gift
    // create disappear action
    this->runAction(Sequence::create(
            CallFunc::create([](){
                MainScene::getInstance()->wait(false);
            }),
            CallFunc::create([](){
                MainScene::getInstance()->popAndReplace();
            }),
            RemoveSelf::create(true),
            NULL
    ));
}

bool Gift::checkTouch(Vec2 point) {
    Vec3* p3 = new Vec3(point.x, point.y, 0);
    _box->getWorldToNodeTransform().transformPoint(p3);
    Vec2 p = Vec2(p3->x, p3->y);
    auto rect =  Rect(0, 0,_box->getContentSize().width, _box->getContentSize().height);
    return _isEnabled && rect.containsPoint(p);
}

void Gift::enable() {
    _isEnabled = true;
    // TODO add appearing action
}
