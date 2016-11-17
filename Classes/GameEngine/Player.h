//
// Created by igor on 05.10.16.
//

#ifndef ARCUNLIM_PLAYER_H
#define ARCUNLIM_PLAYER_H


#include <cocos2d.h>

class Player {
public:
    Player(int hp, const char *name);

private:

    int _id;
    const char *_name;
    int _shotsCount;
    int _hp;

    cocos2d::Node* _view;
    cocos2d::Label* _name_view;
    cocos2d::Label* _hp_view ;
    cocos2d::Label* _shots_view;

    void updateView();
public:

    void setHAlignment(cocos2d::TextHAlignment alignment);

    int getShotsCount() const;

    void addShotsCount();

    int getId() const;

    const char * getName() const;

    cocos2d::Node* getView();

    int getHp() const;

    void setHp(int diff);
};


#endif //ARCUNLIM_PLAYER_H
