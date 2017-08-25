//
// Created by itrofimenko on 17.12.16.
//

#ifndef ARCHERSUNLIMITED_STATISTICS_H
#define ARCHERSUNLIMITED_STATISTICS_H

#include "cocos2d.h"

USING_NS_CC;

class Statistics {
public:
    static Statistics *create(int type);

    bool init(int type);

    int getLevel();

    void increaseLevel(int level);

    int getCoinsGained();

    void increaseCoins(int diff);

    long getTimeSpent();

    void setTime(long timeSpent);

    float getHp();

    void setHp(float hp);

    int getType();

    void setWin();

    bool isVictory();

    int getPlayerEnvType();
    void setPlayerEnvType(int type);

    int getTargetEnvType();
    void setTargetEnvType(int type);

    Node* getView(cocos2d::Size contentSize);

protected:
    bool _victory;
    int _type;
    int _level;
    int _coinsGained;
    long _timeSpent;
    float _hp;
    Node* _view;

    int _playerEnv;
    int _targetEnv;
};


#endif //ARCHERSUNLIMITED_STATISTICS_H
