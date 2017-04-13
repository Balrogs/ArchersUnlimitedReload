#ifndef ARCHERSUNLIMITED_CLOCKS_H
#define ARCHERSUNLIMITED_CLOCKS_H

#include "cocos2d.h"

USING_NS_CC;


class Clocks : public Node {
public:

    static Clocks* create(std::function<void()> callback);

    static Clocks* create(long end, std::function<void()> callback);

    bool init(long end, std::function<void()> callback);

    void update(float dt);

    long getCurrent();
    long timeLeft();

    Size getContentSize();

private:
    Label* _clock;
    std::function<void()> _callback;
    long _startTime;
    long _endTime;
    long _current;

    std::string _convertTimestamp(long timestamp);

};


#endif //ARCHERSUNLIMITED_CLOCKS_H
