#ifndef ARCHERSUNLIMITED_APPLEPARENT_H
#define ARCHERSUNLIMITED_APPLEPARENT_H


class AppleParent {
public:

    virtual void completeShot();

    virtual void setHit();

    virtual void setAppleHit();

    virtual void _nextLevelAction() = 0;

    virtual void addApple(Body *target);

    void removeTarget(Node *target);

protected:

    bool _isTargetHitted;

    bool _isAppleHitted;

    int _shotsLimit;

    int _completedShots;

    std::vector<cocos2d::Node *> _targets;

    cocos2d::Node* _env;
};

#endif //ARCHERSUNLIMITED_APPLEPARENT_H
