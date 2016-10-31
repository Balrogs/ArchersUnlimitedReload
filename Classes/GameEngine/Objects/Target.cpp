//
// Created by igor on 09.09.16.
//

#include <GameEngine/Objects/Arrows/Arrow.h>
#include "Target.h"


Target::Target() : DragonObject(){

}

float Target::getHP() {
    return (_hp > 0) ? _hp : 0;
}

void Target::dealDamage(float hit) {
    _hp -= hit;
}