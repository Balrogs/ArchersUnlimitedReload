#include "EventInfo.h"

EventInfo::EventInfo(int id, int type, long endTime, std::vector<InfoButton*> infos, std::vector<Reward*> rewards, std::vector<EventScore*> scores) :
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

std::vector<Reward *> EventInfo::getRewards() {
    return _rewards;
}

std::vector<EventScore *> EventInfo::getScores() {
    return _scores;
}

int EventInfo::getId() {
    return _id;
}

EventScore::EventScore(std::string name, int score):
        _name(name),
        _score(score){
}

int EventScore::getScore() {
    return _score;
}

std::string EventScore::getName() {
    return _name;
}

bool Reward::init(std::string text, std::string path) {
    return InfoButton::init(text, path);
}

// TODO path and desc for all !!!

RewardCoin *RewardCoin::create(int coins) {
    RewardCoin *ret = new(std::nothrow) RewardCoin();
    if (ret && ret->init("", "")) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

RewardHat *RewardHat::create() {
    RewardHat *ret = new(std::nothrow) RewardHat();
    if (ret && ret->init("", "")) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

RewardArrow *RewardArrow::create() {
    RewardArrow *ret = new(std::nothrow) RewardArrow();
    if (ret && ret->init("", "")) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

RewardBow *RewardBow::create() {
    RewardBow *ret = new(std::nothrow) RewardBow();
    if (ret && ret->init("", "")) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
