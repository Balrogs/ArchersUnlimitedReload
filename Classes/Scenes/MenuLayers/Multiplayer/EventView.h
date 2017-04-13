#ifndef ARCHERSUNLIMITED_EVENTVIEW_H
#define ARCHERSUNLIMITED_EVENTVIEW_H

#include <ui/UIButton.h>
#include <GameEngine/Global/Misc/UI/RichEventButton.h>
#include "cocos2d.h"

class EventView : public cocos2d::Layer {
public:
    CREATE_FUNC(EventView);

    bool init();

protected:

    void _updateEvent();

    void onEnter() override;

    void onQuit();

    cocos2d::Sprite *_bg;
    RichEventButton* _richEventButton;

    cocos2d::Size _visibleSize;
    cocos2d::UserDefault* _def;
};



#endif //ARCHERSUNLIMITED_EVENTVIEW_H
