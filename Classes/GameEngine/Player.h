//
// Created by igor on 05.10.16.
//

#ifndef ARCUNLIM_PLAYER_H
#define ARCUNLIM_PLAYER_H


#include <cocos2d.h>
enum HPState{
    FULL,
    NORMAL,
    DANGER,
    NONE
};

class HPBar : public cocos2d::Node{
public:
    static HPBar* create(cocos2d::Size size);
    bool init(cocos2d::Size size);
    void setHp(float hp);
    void setAlignment(bool alignment);
protected:
    bool _alignment;
    float _hp;
    cocos2d::Size _spriteSize;
    cocos2d::Size _prevSize;
    cocos2d::Size _size;
    cocos2d::Sprite* _bar;
    HPState _state;
    void changeState(HPState state);
};

class Player : public cocos2d::Node{
protected:
    int _id;
    std::string _name;
    int _shotsCount;
    int _hp;

    cocos2d::Label *_name_view;
    HPBar *_hp_view;
    cocos2d::Label *_shots_view;

    void updateView();

public:

    static Player* create(int id, int hp, std::string name);
    static Player* create(int hp, std::string name);

    virtual bool init(int id, int hp, std::string name);

    void setHAlignment(cocos2d::TextHAlignment alignment);

    int getShotsCount();

    void addShotsCount();

    void nullShotsCount();

    int getId();

    std::string getName();

    int getHp();

    void setHp(int diff);
};


#endif //ARCUNLIM_PLAYER_H
