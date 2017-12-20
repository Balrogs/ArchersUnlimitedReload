#ifndef ARCUNLIM_DAILYGIFTSCENE_H
#define ARCUNLIM_DAILYGIFTSCENE_H

#include <ui/UIScrollView.h>
#include <ui/UIButton.h>
#include "cocos2d.h"


class DailyGiftScene : public cocos2d::Layer {
public:
    CREATE_FUNC(DailyGiftScene);

    bool init();

    static bool checkDate();
    static const long DAY_IN_MILLIS;

protected:

    cocos2d::UserDefault *_def;

    void _generateList(int day, cocos2d::Size size);

    void _setItems(std::vector<int> items, int day, cocos2d::Size size);

    void _addItem(int id, cocos2d::Vec2 position, cocos2d::Size size, bool enabled);

    int  _getDay();

    void onEnter() override;

    void onQuit();

    cocos2d::Sprite *_bg;

    cocos2d::Size _visibleSize;
};



#endif //ARCUNLIM_DAILYGIFTSCENE_H
