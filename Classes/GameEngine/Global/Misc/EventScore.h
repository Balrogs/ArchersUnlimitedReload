#ifndef ARCHERSUNLIMITED_EVENTSCORE_H
#define ARCHERSUNLIMITED_EVENTSCORE_H


class EventScore {
public:
    EventScore(std::string name, int score);
    int getScore();
    std::string getName();
private:
    std::string _name;
    int _score;
};

#endif //ARCHERSUNLIMITED_EVENTSCORE_H
