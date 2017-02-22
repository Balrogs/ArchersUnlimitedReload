#ifndef ARCHERSUNLIMITED_RICHSEARCHBUTTON_H
#define ARCHERSUNLIMITED_RICHSEARCHBUTTON_H

#include "cocos2d.h"

USING_NS_CC;

class RichSearchButton : public Node {
public:
    CREATE_FUNC(RichSearchButton);

    bool init() override;

    void reset();

    void start();

protected:

    enum State{
        Idle,
        Searching
    };

    State _state;
    Sprite* _stopButton;

    Layer* _lobby;

    float _scale;

    void _switchState(State state);

};


#endif //ARCHERSUNLIMITED_RICHSEARCHBUTTON_H
