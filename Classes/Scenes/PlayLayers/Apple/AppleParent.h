#ifndef ARCHERSUNLIMITED_APPLEPARENT_H
#define ARCHERSUNLIMITED_APPLEPARENT_H


class AppleParent {
public:

    virtual void completeShot();

    virtual void setHit();

    virtual void setAppleHit();

    virtual void _nextLevelAction() = 0;

    virtual void addApple() = 0;

    void removeTarget(Node *target);

protected:

    bool _isTargetHitted;

    bool _isAppleHitted;

    int _shotsLimit;

    int _completedShots;

    std::vector<cocos2d::Node *> _targets;
};

#endif //ARCHERSUNLIMITED_APPLEPARENT_H
