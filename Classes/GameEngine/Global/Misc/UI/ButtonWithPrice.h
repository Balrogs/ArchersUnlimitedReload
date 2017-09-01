#ifndef ARCHERSUNLIMITED_BUTTONWITHPRICE_H
#define ARCHERSUNLIMITED_BUTTONWITHPRICE_H

#include "cocos2d.h"

USING_NS_CC;

class ButtonWithPrice : public Node {
public:

    static ButtonWithPrice* create(int cost);

    bool init(int cost);

    void addAction(std::function<void()> callback);

    void setEnable(bool enable);

protected:

    std::function<void()> _callback;
    cocos2d::UserDefault* _def;
    int _cost;
    ui::Button* _coins_bar;

    bool _checkEnable();
};


#endif //ARCHERSUNLIMITED_BUTTONWITHPRICE_H
