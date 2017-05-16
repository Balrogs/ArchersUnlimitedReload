#include <string>
#include "EventScore.h"

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
