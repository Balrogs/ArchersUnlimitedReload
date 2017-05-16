#include "Reward.h"

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
