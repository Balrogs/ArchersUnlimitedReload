#ifndef ARCHERSUNLIMITED_REGISTERMENU_H
#define ARCHERSUNLIMITED_REGISTERMENU_H

#include <ui/UIScrollView.h>
#include "cocos2d.h"

class RegisterMenu : public cocos2d::Layer {
public:
    CREATE_FUNC(RegisterMenu);

    bool init();

    void onError(string message);

protected:
    cocos2d::Size _visibleSize;
    cocos2d::Sprite *_bg;
    cocos2d::ui::EditBox *_editName;
    cocos2d::ui::EditBox *_editPassword;
    std::string _region;
    cocos2d::Label *_regionBox_title;
    cocos2d::ui::Button *_regionBox;
    cocos2d::ui::ScrollView *_scrollView;
    cocos2d::Label *_errorMessage;
    SocketClient *_client;

    void onEnter() override;
    void onQuit();
    void _showScrollView();
};


#endif //ARCHERSUNLIMITED_REGISTERMENU_H
