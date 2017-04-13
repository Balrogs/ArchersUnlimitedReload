
#ifndef ARCHERSUNLIMITED_RICHEVENTBUTTON_H
#define ARCHERSUNLIMITED_RICHEVENTBUTTON_H

#include "cocos2d.h"
#include "Clocks.h"

USING_NS_CC;

class RichEventButton : public Node {
public:
    CREATE_FUNC(RichEventButton);

    bool init() override;

    void saveTime();

    void reset();
    void start();

    void increaseTryCount();
    void decreaseTryCount();

protected:

    enum State{
        Idle,
        Waiting
    };

    State _state;
    int _tryCount;
    float _scale;
    long _timeToWait;

    Clocks* _clocks;
    Label* _tryLabel;
    Sprite* _movieButton;
    cocos2d::UserDefault* _def;

    void _onCreate();

    void _switchState(State state);
    void _updateTryCount();
};


#endif //ARCHERSUNLIMITED_RICHEVENTBUTTON_H
