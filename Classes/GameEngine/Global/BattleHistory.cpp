#include <base/CCConsole.h>
#include "BattleHistory.h"

Round::Round(int playerId) {
    _id = BattleHistory::getInstance()->getRoundId();
    _playerId = playerId;
}

float Round::getHitInfo() {
    return _hitInfo;
}

void Round::setHitInfo(float hitInfo) {
    _hitInfo = hitInfo;
}


void Round::copy(Round *round) {
    _hitInfo = round->getHitInfo();
}

bool Round::compare(Round *round) {
    return _id == round->getId() && _playerId == round->getPlayerId();
}

int Round::getId() {
    return _id;
}

int Round::getPlayerId() {
    return  _playerId;
}

BattleHistory* BattleHistory::_instance = nullptr;

Round *BattleHistory::getRoundInfo(int id, int playerId) {
    for(auto r : _rounds){
        if(r->getId() == id && r->getPlayerId() == playerId){
            return r;
        }
    }
    return nullptr;
}

void BattleHistory::addRound(Round *round) {
    for(auto r : _rounds){
        if(r->compare(round)){
            r->copy(round);
            return;
        }
    }
    _rounds.push_back(round);
}

BattleHistory *BattleHistory::getInstance() {
   if(_instance == nullptr){
       _instance = new BattleHistory();
   }
    return _instance;
}

int BattleHistory::getRoundId() {
    return _roundId;
}

void BattleHistory::nextRound() {
    _roundId++;
}

std::vector<Round *> BattleHistory::getAllRounds() {
    return _rounds;
}
