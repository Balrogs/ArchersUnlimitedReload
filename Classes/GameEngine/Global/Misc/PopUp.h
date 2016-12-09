//
// Created by igor on 05.12.16.
//

#ifndef ARCHERSUNLIMITED_POPUP_H
#define ARCHERSUNLIMITED_POPUP_H

#include "cocos2d.h"
#include "ui/UIButton.h"

class PopUp : public cocos2d::Node {
public:
    static PopUp *create(std::string title, cocos2d::Node *message, bool isTwoButtons);

    static PopUp *create(std::string title, cocos2d::Node *message);

    virtual bool init(std::string title);

    virtual bool init(std::string title, cocos2d::Node *message);

    virtual bool init(std::string title, cocos2d::Node *message, bool isTwoButtons);

    cocos2d::Size POPUP_SIZE;

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

class PausePopUp : public PopUp {
public:
    static PausePopUp *create(std::string title);
    bool init(std::string title);

protected:
    cocos2d::ui::Button *_musicButton;
    cocos2d::ui::Button *_effectsButton;
    bool _musicState;
    bool _effectsState;

    void _reloadButtons();

    virtual void noAction() ;

    virtual void yesAction();
};

#endif //ARCHERSUNLIMITED_POPUP_H
