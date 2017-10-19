//
// Created by igor on 05.12.16.
//

#ifndef ARCHERSUNLIMITED_POPUP_H
#define ARCHERSUNLIMITED_POPUP_H

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "ui/UIEditBox/UIEditBox.h"

class PopUp : public cocos2d::Node {
public:
    static PopUp *create(std::string title, cocos2d::Node *message, bool isTwoButtons);

    static PopUp *create(std::string title, cocos2d::Node *message);

    virtual bool init(std::string title);

    virtual bool init(std::string title, cocos2d::Node *message);

    virtual bool init(std::string title, cocos2d::Node *message, bool isTwoButtons);

    cocos2d::Size POPUP_SIZE;

    virtual void onEnterTransitionDidFinish() override;

    virtual void removeFromParentAndCleanup(bool cleanup) override ;

protected:

    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _scale;
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

class InvitePopUp : public PopUp {
public:
    static InvitePopUp *create(std::string title, cocos2d::Node *message, bool isTwoButtons) ;

    static InvitePopUp *create(std::string title, cocos2d::Node *message);

    bool init(std::string title, cocos2d::Node *message) ;

    bool init(std::string title, cocos2d::Node *message, bool isTwoButtons);

protected:
    void noAction() override ;

    void yesAction() override;

    void okAction() override;
};

class GameTypePopUp : public PopUp {
public:
    static GameTypePopUp *create();
    bool init();

    static GameTypePopUp *create(std::string name, int id);
    bool init(std::string name, int id);

protected:
    std::string _name;
    int _id;
};

class InputNamePopUp : public PopUp {
public:
    static InputNamePopUp *create();
    bool init();

protected:
    cocos2d::Label *_errorMessage;
    cocos2d::ui::EditBox *_editName;

    virtual void okAction() override;
};

class WaitingPopUp : public PopUp {
public:
    static WaitingPopUp *create();
    static WaitingPopUp *create(int time);

    bool init(int time);

    void update(float dt);

protected:
    int _waitingTime;
    int _counter = 0;
    cocos2d::Label *_message;
    cocos2d::ui::Button *_ok;
};

class RateUsPopUp : public PopUp {
public:
    CREATE_FUNC(RateUsPopUp);
    bool init() override;

protected:

    void noAction() override ;

    void yesAction() override;

    void okAction() override;
};


#endif //ARCHERSUNLIMITED_POPUP_H
