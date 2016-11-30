//
// Created by igor on 05.10.16.
//

#ifndef ARCUNLIM_PLAYER_H
#define ARCUNLIM_PLAYER_H


#include <cocos2d.h>

class Player : public cocos2d::Node{

protected:

    int _id;
    std::string _name;
    int _shotsCount;
    int _hp;

    cocos2d::Label *_name_view;
    cocos2d::Label *_hp_view;
    cocos2d::Label *_shots_view;

    cocos2d::TextHAlignment _alignment;

    void updateView();

public:
    Player(int id, int hp, std::string name);
    Player(int hp, std::string name);

    void setHAlignment(cocos2d::TextHAlignment alignment);

    int getShotsCount();

    void addShotsCount();

    int getId();

    std::string getName();

    int getHp();

    void setHp(int diff);
};


#endif //ARCUNLIM_PLAYER_H
