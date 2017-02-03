#ifndef ARCHERSUNLIMITED_REGISTERMENU_H
#define ARCHERSUNLIMITED_REGISTERMENU_H

#include "cocos2d.h"

class RegisterMenu : public cocos2d::Layer {
public:
    CREATE_FUNC(RegisterMenu);

    bool init();

protected:
    cocos2d::Size _visibleSize;

    cocos2d::ui::EditBox *_editName;
    cocos2d::ui::EditBox *_editPassword;
    cocos2d::Label *_errorMessage;
    SocketClient *_client;

    void onEnter() override;
    void onQuit();
};


#endif //ARCHERSUNLIMITED_REGISTERMENU_H
