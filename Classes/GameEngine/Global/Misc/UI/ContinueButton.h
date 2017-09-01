#ifndef ARCHERSUNLIMITED_CONTINUEBUTTON_H
#define ARCHERSUNLIMITED_CONTINUEBUTTON_H

#include <ui/UIButton.h>
#include "cocos2d.h"
#include "ButtonWithPrice.h"

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
    Node* _price;
    Sprite* _loading;
};

#endif //ARCHERSUNLIMITED_CONTINUEBUTTON_H
