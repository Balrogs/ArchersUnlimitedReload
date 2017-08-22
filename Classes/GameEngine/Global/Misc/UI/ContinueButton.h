#ifndef ARCHERSUNLIMITED_CONTINUEBUTTON_H
#define ARCHERSUNLIMITED_CONTINUEBUTTON_H

#include <ui/UIButton.h>
#include "cocos2d.h"

USING_NS_CC;

class ContinueButton : public Node {
public:

    CREATE_FUNC(ContinueButton);

    bool init();

    ui::Button* button();

    Size getBoundingBox();

    void enable();


protected:
    bool _enabled;
    ui::Button* _button;
    Sprite* _ad;
    Sprite* _loading;
};

#endif //ARCHERSUNLIMITED_CONTINUEBUTTON_H
