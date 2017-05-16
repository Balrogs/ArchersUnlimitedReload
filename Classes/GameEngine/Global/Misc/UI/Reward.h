#ifndef ARCHERSUNLIMITED_REWARD_H
#define ARCHERSUNLIMITED_REWARD_H


#include <string>
#include "InfoButton.h"

class Reward : public InfoButton {
protected:
    bool init(std::string text, std::string path);
};

class RewardCoin : public Reward {
public:
    static RewardCoin* create(int coins);
};

class RewardHat : public Reward {
public:
    static RewardHat* create();
};

class RewardArrow : public Reward {
public:
    static RewardArrow* create();
};

class RewardBow : public Reward {
public:
    static RewardBow* create();
};

#endif //ARCHERSUNLIMITED_REWARD_H
