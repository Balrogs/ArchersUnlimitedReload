#ifndef ARCHERSUNLIMITED_BATTLEHISTORY_H
#define ARCHERSUNLIMITED_BATTLEHISTORY_H

#include <vector>


class Round {
public:
    Round(int playerId);

    void copy(Round* round);

    bool compare(Round* round);

    int getId();
    int getPlayerId();

    float getHitInfo();

    void setHitInfo(float hitInfo);

private:
    int _id;
    int  _playerId;
    float _hitInfo;
};

class BattleHistory {
public:
    static BattleHistory* getInstance();

    Round* getRoundInfo(int id, int playerId);

    std::vector<Round*> getAllRounds();

    int getRoundId();

    void nextRound();

    void addRound(Round* round);

private:

    int _roundId;

    static BattleHistory* _instance;

    std::vector<Round*> _rounds;
};


#endif //ARCHERSUNLIMITED_BATTLEHISTORY_H
