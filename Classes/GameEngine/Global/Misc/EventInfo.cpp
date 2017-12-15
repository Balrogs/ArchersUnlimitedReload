#include "EventInfo.h"

EventInfo::EventInfo(int id, int type, long endTime, std::vector<InfoButton*> infos, std::vector<RewardInfo*> rewards, std::vector<EventScore*> scores) :
        _id(id),
        _type(type),
        _endTime(endTime),
        _description(infos),
        _rewards(rewards),
        _scores(scores) {

}

int EventInfo::getType() {
    return _type;
}

long EventInfo::getEndTime() {
    return _endTime;
}

std::vector<InfoButton *> EventInfo::getInfo() {
    return _description;
}

std::vector<RewardInfo *> EventInfo::getRewards() {
    return _rewards;
}

std::vector<EventScore *> EventInfo::getScores() {
    return _scores;
}

int EventInfo::getId() {
    return _id;
}
