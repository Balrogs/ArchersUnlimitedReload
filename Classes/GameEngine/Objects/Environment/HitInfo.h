#ifndef ARCHERSUNLIMITED_HITINFO_H
#define ARCHERSUNLIMITED_HITINFO_H


#include <cocos2d.h>

USING_NS_CC;

class HitInfo : public Node {
protected:
    bool init(Vec2 pos);
};


class DamageInfo: public HitInfo {
public:
    static DamageInfo* create(Vec2 pos, int count);

protected:
    bool init(Vec2 pos, int count);
};

class CoinInfo: public HitInfo {
public:
    static CoinInfo* create(Vec2 pos, int count);

protected:
    bool init(Vec2 pos, int count);
};



#endif //ARCHERSUNLIMITED_HITINFO_H
