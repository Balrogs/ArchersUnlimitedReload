#ifndef ARCHERSUNLIMITED_INFOBUTTON_H
#define ARCHERSUNLIMITED_INFOBUTTON_H

#include "cocos2d.h"

class InfoButton : public cocos2d::Node {
public:
    void show();

    void hide();

protected:
    virtual bool init(std::string text,std::string buttonPath);

    enum State{
        Idle,
        Showing
    };

    State _state;

    cocos2d::Sprite* _cloud;
    cocos2d::Vec2 _idlePos;
    cocos2d::Vec2 _showingPos;

    void _switchState(State state);
};


class InfoButtonTime : public InfoButton {
public:
    static InfoButtonTime* create();
};

class RewardInfo : public InfoButton {
protected:
    bool init(std::string text, std::string path);
};

class RewardInfoCoin : public RewardInfo {
public:
    static RewardInfoCoin* create(int coins);
    bool init(std::string text, std::string path) override;
};

class RewardInfoHat : public RewardInfo {
public:
    static RewardInfoHat* create();
};

class RewardInfoArrow : public RewardInfo {
public:
    static RewardInfoArrow* create();
};

class RewardInfoBow : public RewardInfo {
public:
    static RewardInfoBow* create();
};


#endif //ARCHERSUNLIMITED_INFOBUTTON_H
