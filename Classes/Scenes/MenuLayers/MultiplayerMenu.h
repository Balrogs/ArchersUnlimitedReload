
#ifndef ARCHERSUNLIMITED_MULTIPLAYERMENU_H
#define ARCHERSUNLIMITED_MULTIPLAYERMENU_H

#include <GameEngine/Global/Misc/SocketClient.h>
#include <ui/UIEditBox/UIEditBox.h>
#include "cocos2d.h"

class MultiplayerMenu : public cocos2d::Layer {
public:

    CREATE_FUNC(MultiplayerMenu);

    bool init();

    virtual void onError(string message);

    virtual void onQuit();

protected:
    void onMenuClick(int id);

    void update(float dt);

    cocos2d::Label *_errorMessage;
    SocketClient *_client;

private:
    cocos2d::ui::EditBox *_editName;
    cocos2d::ui::EditBox *_editPassword;
};

#endif //ARCHERSUNLIMITED_MULTIPLAYERMENU_H
