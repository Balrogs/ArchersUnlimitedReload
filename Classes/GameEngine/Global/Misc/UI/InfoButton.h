#ifndef ARCHERSUNLIMITED_INFOBUTTON_H
#define ARCHERSUNLIMITED_INFOBUTTON_H

#include "cocos2d.h"

USING_NS_CC;

class InfoButton : public Node {
public:
    void show();

    void hide();

protected:
    bool init(std::string text,std::string buttonPath);

    enum State{
        Idle,
        Showing
    };

    State _state;

    Sprite* _cloud;
    Vec2 _idlePos;
    Vec2 _showingPos;

    void _switchState(State state);
};


class InfoButtonTime : public InfoButton {
public:
    static InfoButtonTime* create();
};

#endif //ARCHERSUNLIMITED_INFOBUTTON_H
