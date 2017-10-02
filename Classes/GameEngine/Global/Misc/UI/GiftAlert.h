#ifndef ARCHERSUNLIMITED_GIFTALERT_H
#define ARCHERSUNLIMITED_GIFTALERT_H

#include "cocos2d.h"
#include "Clocks.h"

USING_NS_CC;

class GiftAlert : public Node {
public:
    CREATE_FUNC(GiftAlert);

    bool init() override;

    void saveTime();

    void touch();

    Rect getBoundingBox() const override;

protected:

    void _reset();

    enum State {
        ON,
        OFF
    };

    State _state;
    long _timeToWait;

    Clocks* _clocks;
    cocos2d::UserDefault* _def;

    Sprite* _alertOn;
    Sprite* _alertOff;
    Sprite* _board;

    void _onCreate();

    void _showPopUp();

    void _switchState(State state);
};


#endif //ARCHERSUNLIMITED_GIFTALERT_H
