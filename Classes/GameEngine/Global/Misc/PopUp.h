//
// Created by igor on 05.12.16.
//

#ifndef ARCHERSUNLIMITED_POPUP_H
#define ARCHERSUNLIMITED_POPUP_H

#include "cocos2d.h"

class PopUp : public cocos2d::Node {
public:
    static PopUp *create(std::string title, cocos2d::Node *message, bool isTwoButtons);

    static PopUp *create(std::string title, cocos2d::Node *message);

    bool init(std::string title);

    bool init(std::string title, cocos2d::Node *message);

    bool init(std::string title, cocos2d::Node *message, bool isTwoButtons);

    static const cocos2d::Size POPUP_SIZE;

protected:
    cocos2d::Label *_title;
    cocos2d::Node *_buttons;

    virtual void noAction();

    virtual void yesAction();

    virtual void okAction();
};

class MainMenuPopUp : public PopUp {
public:
    static MainMenuPopUp *create(std::string title, cocos2d::Node *message, bool isTwoButtons) ;

    static MainMenuPopUp *create(std::string title, cocos2d::Node *message);

protected:
    void noAction() override ;

    void yesAction() override;

    void okAction() override;
};

#endif //ARCHERSUNLIMITED_POPUP_H
