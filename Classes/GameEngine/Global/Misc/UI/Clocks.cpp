#include <GameEngine/Global/Variables.h>
#include "Clocks.h"

Clocks *Clocks::create(std::function<void()> callback) {
    return create(callback, 3);
}

Clocks *Clocks::create(long end, std::function<void()> callback) {
    return create(end, callback, 3);
}


Clocks *Clocks::create(std::function<void()> callback, int dim) {
    long end = Variables::getCurrentTime() + Variables::RESET_INTERVAL;

    Clocks *ret = new(std::nothrow) Clocks();
    if (ret && ret->init(end, callback, dim)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

Clocks *Clocks::create(long end, std::function<void()> callback, int dim) {
    Clocks *ret = new(std::nothrow) Clocks();
    if (ret && ret->init(end, callback, dim)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}



bool Clocks::init(long end, std::function<void()> callback, int dim) {
    if(!Node::init()){
        return false;
    }
    _dim = dim;

    _callback = callback;

    _endTime = end;

    _current = Variables::getCurrentTime();

    _clock = cocos2d::Label::createWithTTF("", Variables::FONT_NAME, Variables::FONT_SIZE());
    this->addChild(_clock);

    this->schedule(SEL_SCHEDULE(&Clocks::update), 1.f);

    return true;
}

void Clocks::update(float dt) {
    Node::update(dt);

    _current += 1000;

    if(_current < _endTime){
        _clock->setString(_convertTimestamp(_endTime - _current));
    } else{
       _callback();
    }
}

std::string Clocks::_convertTimestamp(long timestamp) {
    switch(_dim){
        case 2:{
            int seconds = (int) (timestamp / 1000) % 60 ;
            int minutes = (int) ((timestamp / (1000*60)) % 60);
            return StringUtils::format("%02d:%02d", minutes, seconds);
        }
        case 3:{
            int seconds = (int) (timestamp / 1000) % 60 ;
            int minutes = (int) ((timestamp / (1000*60)) % 60);
            int hours   = (int) ((timestamp / (1000*60*60)) % 24);
            return StringUtils::format("%02d:%02d:%02d", hours, minutes, seconds);
        }
        default:
            return "";
    }

}

Size Clocks::getContentSize() {
    return _clock->getContentSize();
}

long Clocks::getCurrent() {
    return _current;
}

long Clocks::timeLeft() {
    return _endTime - _current;
}



