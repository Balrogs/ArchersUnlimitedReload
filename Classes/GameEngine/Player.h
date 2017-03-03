#ifndef ARCUNLIM_PLAYER_H
#define ARCUNLIM_PLAYER_H


#include <cocos2d.h>

enum HPState {
    FULL,
    NORMAL,
    DANGER,
    NONE
};

class HPBar : public cocos2d::Node {
public:
    static HPBar *create(cocos2d::Size size);

    bool init(cocos2d::Size size);

    void setHp(float hp);

    void setAlignment(bool alignment);

protected:
    bool _alignment;
    float _hp;
    cocos2d::Size _spriteSize;
    cocos2d::Size _prevSize;
    cocos2d::Size _size;
    cocos2d::Sprite *_bar;
    HPState _state;

    void changeState(HPState state);
};

class Player : public cocos2d::Node {
public:
    virtual bool init(int id, std::string name);

    int getId();
    std::string getName();

    virtual std::string getPlayerView() = 0;
    virtual int getHp() = 0;
    virtual void setHAlignment(cocos2d::TextHAlignment alignment) = 0;

protected:
    int _id;
    std::string _name;
    cocos2d::Size _visibleSize;

    virtual void _updateView() = 0;

};

class Bot : public Player {

public:
    static Bot *create(std::string name, int hp);

    virtual bool init(int id, std::string name, int hp);

    int getHp();

    void setHp(int diff);

    virtual void setHAlignment(cocos2d::TextHAlignment alignment);

    virtual std::string getPlayerView();

protected:
    int _hp;
    HPBar *_hpView;
    virtual void _updateView();
};


class PlayerDuel : public Player {
public:
    static PlayerDuel *create(int id, std::string name, int hp = 100, int shotsCount = 0);

    virtual bool init(int id, std::string name, int hp, int shotsCount);

    virtual void setHAlignment(cocos2d::TextHAlignment alignment);

    int getShotsCount();

    void addShotsCount();

    void nullShotsCount();

    int getHp();

    void setHp(int diff);

    virtual std::string getPlayerView();

    void markTurn(bool p);

protected:
    cocos2d::Label *_nameView;
    virtual void _updateView();
private:

    int _shotsCount;
    int _hp;

    cocos2d::Label *_shotsView;
    HPBar *_hpView;

};


class PlayerApple : public PlayerDuel {
public:

    static PlayerApple *create(int id, std::string name,  int score = 0);

    virtual bool init(int id, std::string name, int score);

    void addScore(int newCount);

    virtual std::string getPlayerView();

    virtual  int getHp();

    virtual void setHAlignment(cocos2d::TextHAlignment alignment);

protected:
    cocos2d::Label *_nameView;

    cocos2d::Node* _scoreView;

    virtual void _updateView();
};

class PlayerOnlineApple : public PlayerApple {
public:
    static PlayerOnlineApple *create(int id, std::string name, int score = 0);

    bool init(int id, std::string name, int score);

    virtual std::string getPlayerView();

protected:

    virtual void _updateView();
};

#endif //ARCUNLIM_PLAYER_H
