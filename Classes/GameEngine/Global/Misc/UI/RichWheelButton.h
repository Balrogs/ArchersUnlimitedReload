#ifndef ARCHERSUNLIMITED_RICHWHEELBUTTON_H
#define ARCHERSUNLIMITED_RICHWHEELBUTTON_H

#include "cocos2d.h"

USING_NS_CC;

class RichWheelButton : public Node {
public:
    CREATE_FUNC(RichWheelButton);

    bool init() override;

    Size getBoundingBox();

protected:

    ui::Button* _button;
    float _scale;
};

#endif //ARCHERSUNLIMITED_RICHWHEELBUTTON_H
