#ifndef ARCHERSUNLIMITED_EVENTVIEW_H
#define ARCHERSUNLIMITED_EVENTVIEW_H

#include <ui/UIButton.h>
#include <GameEngine/Global/Misc/UI/RichEventButton.h>
#include <GameEngine/Global/Misc/SocketClient.h>
#include <GameEngine/Global/Misc/EventInfo.h>
#include "cocos2d.h"

class EventView : public cocos2d::Layer {
public:
    CREATE_FUNC(EventView);

    bool init();

    void updateEvent(EventInfo* info);

protected:

    void _updateEvent();

    void onEnter() override;

    void onQuit();

    cocos2d::Sprite *_bg;
    RichEventButton* _richEventButton;

    EventInfo* _info;
    Clocks* _clocks;

    cocos2d::Size _visibleSize;
    cocos2d::UserDefault* _def;
    SocketClient* _client;
    Label *_title;
};



#endif //ARCHERSUNLIMITED_EVENTVIEW_H
