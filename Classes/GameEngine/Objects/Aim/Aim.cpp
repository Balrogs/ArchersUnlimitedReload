//
// Created by igor on 08.09.16.
//

#include "Aim.h"
#include "Scenes/PlayLayers/Battle.h"

USING_NS_CC;

const float Aim::AIM_DISTANCE_X = 200.f;
const float Aim::AIM_DISTANCE_Y = 200.f;

Aim::Aim() : _aimDir(10),
             _aimRadian(0),
             _aiming(false),
             _aimPower(5.f),
             _aimPreviewCount(10){
    BattleScene::instance->addChild(this);
}

Aim::~Aim() {
}

void Aim::_update() {
    int i = 0;
    auto children = this->getChildren();
    for (auto child : children) {
        if (AimPreview *aimP = dynamic_cast<AimPreview *>(child)) {
            aimP->update(setScale(i), setPosition(i));
            i++;
        }
    }
}


void Aim::_addAimPreview(float scale, const cocos2d::Vec2 &position) {
    this->addChild(new AimPreview(scale, position));
}

bool Aim::is_aiming() const {
    return _aiming;
}

float Aim::get_aimRadian() const {
    return _aimRadian;
}

void Aim::set_aimPoint(Vec2 _aimPoint) {
    Aim::_aimPoint = _aimPoint;
    _update();
}

void Aim::set_aimDir(int _aimDir) {
    Aim::_aimDir = _aimDir;
    _update();
}

void Aim::set_aiming(bool _aiming) {
    if (!_aiming) {
        _aimPower = 5.f;
        _aimRadian = 0.f;
        auto children = this->getChildren();
        for (auto child : children) {
            if (AimPreview *aimP = dynamic_cast<AimPreview *>(child)) {
                aimP->remove();
            }
        }
    } else {
        for (int i = 0; i < _aimPreviewCount; i++) {
            _addAimPreview(setScale(i), setPosition(i));
        }

    }
    Aim::_aiming = _aiming;
}

void Aim::set_aimRadian(float _aimRadian) {
    Aim::_aimRadian = _aimRadian;
    _update();
}

void Aim::set_aimPower(float _aimPower) {
    Aim::_aimPower = _aimPower;
    _update();
}

float Aim::get_aimPower() const {
    return _aimPower;
}

float Aim::setScale(int i) {
    if (i < _aimPreviewCount / 3) {
        return 0.2f + 0.2f * i;
    } else if (i > _aimPreviewCount * 2 / 3) {
        i = (int) (_aimPreviewCount - i);
        return 0.2f + 0.2f * i;
    }
    return 1.f;
}

cocos2d::Vec2 Aim::setPosition(int i) {
    auto position = _aimPoint;
    auto speedX = std::cos(_aimRadian) * _aimPower;
    auto speedY = -std::sin(_aimRadian) * _aimPower;
    auto rotation = _aimRadian * dragonBones::RADIAN_TO_ANGLE;
    for (int j = 0; j < i; j++) {
        if (rotation >= 180) {
            speedY -= BattleScene::G;
        } else {
            speedY += BattleScene::G;
        }
        rotation = std::atan2(-speedY, speedX) * dragonBones::RADIAN_TO_ANGLE;
    }
    return Vec2(position.x + (i * speedX ), position.y + (i * speedY ));
}

