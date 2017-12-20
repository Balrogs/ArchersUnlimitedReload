#ifndef ARCHERSUNLIMITED_REWARDBUTTON_H
#define ARCHERSUNLIMITED_REWARDBUTTON_H

#include <ui/UIButton.h>
#include "cocos2d.h"

class RewardButton : public cocos2d::Node {
public:

    static RewardButton* create(std::string itemType, int value, std::string buttonPath, std::string buttonPressedPath);
    void setEnabled(bool enabled);
    const cocos2d::Size& getContentSize();
protected:

    virtual bool init(std::string buttonPath, std::string buttonPressedPath);
    cocos2d::ui::Button* _button;
    cocos2d::FiniteTimeAction* _action;
    void _setDate();
    void _onQuit();
};

class RewardButtonArrow : public RewardButton {
public:
    virtual bool init(int value, std::string buttonPath, std::string buttonPressedPath);
};

class RewardButtonBow : public RewardButton {
public:
    virtual bool init(int value, std::string buttonPath, std::string buttonPressedPath);
};

class RewardButtonCoin : public RewardButton {
public:
    virtual bool init(int value, std::string buttonPath, std::string buttonPressedPath);
};

class RewardButtonHat : public RewardButton {
public:
    virtual bool init(int value, std::string buttonPath, std::string buttonPressedPath);
};

class RewardButtonDisabled : public RewardButton {
public:
    CREATE_FUNC(RewardButtonDisabled);
    bool init();
};

#endif //ARCHERSUNLIMITED_REWARDBUTTON_H
