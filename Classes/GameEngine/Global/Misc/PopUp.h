//
// Created by igor on 05.12.16.
//

#ifndef ARCHERSUNLIMITED_POPUP_H
#define ARCHERSUNLIMITED_POPUP_H

#include "cocos2d.h"

class PopUp : public cocos2d::Node
{
public:
    PopUp(std::string title);
    PopUp(std::string title, cocos2d::Node* message, cocos2d::CallFunc *okCallback);
    PopUp(std::string title, cocos2d::Node* message, cocos2d::CallFunc *yesCallback, cocos2d::CallFunc *noCallback);
    void closePopup();

    static const cocos2d::Size POPUP_SIZE;



private:

    cocos2d::Label* _title;
    cocos2d::Menu* _buttons;
    cocos2d::Menu* _close;

    void _popScene();
    void BlockPassingTouch();
};


#endif //ARCHERSUNLIMITED_POPUP_H
