#ifndef ARCHERSUNLIMITED_RICHWHEELBUTTON_H
#define ARCHERSUNLIMITED_RICHWHEELBUTTON_H

#include "cocos2d.h"
#include "Clocks.h"

USING_NS_CC;

class RichWheelButton : public Node {
public:
    CREATE_FUNC(RichWheelButton);

    bool init() override;

    Size getBoundingBox();

protected:

    bool _enabled;
    cocos2d::UserDefault* _def;
    Clocks* _clocks;
    ui::Button* _button;
    Sprite* _wheel;
    Sprite* _highlighting;

    virtual void _showMessage();
    virtual void _checkEnabled();
    void _highlight();
};


class RichWheelButtonCoins : public RichWheelButton {
public:
    CREATE_FUNC(RichWheelButtonCoins);

    bool init() override;

protected:
    void _showMessage() override;
    void _checkEnabled() override;

};

#endif //ARCHERSUNLIMITED_RICHWHEELBUTTON_H
