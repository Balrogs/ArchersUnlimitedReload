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
    cocos2d::UserDefault* _def;
    Clocks* _clocks;
    ui::Button* _button;
    Sprite* _wheel;
    Sprite* _highlighting;

    void _highlight();
};

#endif //ARCHERSUNLIMITED_RICHWHEELBUTTON_H
