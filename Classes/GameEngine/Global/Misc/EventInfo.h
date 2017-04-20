#ifndef ARCHERSUNLIMITED_EVENTINFO_H
#define ARCHERSUNLIMITED_EVENTINFO_H


#include <string>
#include <vector>
#include <GameEngine/Global/Misc/UI/InfoButton.h>

class EventScore {
public:
    EventScore(std::string name, int score);
    int getScore();
    std::string getName();
private:
    std::string _name;
    int _score;
};

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


class EventInfo {
public:

    EventInfo(int id, int type, long endTime, std::vector<InfoButton*> infos, std::vector<Reward*> rewards, std::vector<EventScore*> scores);

    int getId();
    int getType();
    long getEndTime();

    std::vector<InfoButton*> getInfo();
    std::vector<Reward*> getRewards();
    std::vector<EventScore*> getScores();

private:
    int _id;
    int _type;
    long _endTime;
    std::vector<InfoButton*> _description;
    std::vector<Reward*> _rewards;
    std::vector<EventScore*> _scores;
};

#endif //ARCHERSUNLIMITED_EVENTINFO_H
