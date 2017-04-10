#ifndef ARCHERSUNLIMITED_EVENTVIEW_H
#define ARCHERSUNLIMITED_EVENTVIEW_H

#include <ui/UIButton.h>
#include "cocos2d.h"

class EventView : public cocos2d::Layer {
public:
    CREATE_FUNC(EventView);

    bool init();

protected:

    void onEnter() override;

    void onQuit();

    cocos2d::Sprite *_bg;

    cocos2d::Size _visibleSize;
};



#endif //ARCHERSUNLIMITED_EVENTVIEW_H
