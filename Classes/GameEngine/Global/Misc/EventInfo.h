#ifndef ARCHERSUNLIMITED_EVENTINFO_H
#define ARCHERSUNLIMITED_EVENTINFO_H


#include <string>
#include <vector>
#include <GameEngine/Global/Misc/UI/InfoButton.h>
#include <GameEngine/Global/Misc/UI/Reward.h>
#include "EventScore.h"


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
