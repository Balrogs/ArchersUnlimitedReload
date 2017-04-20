#ifndef ARCUNLIM_DRAGONOBJECT_H
#define ARCUNLIM_DRAGONOBJECT_H

#include <dragonBones/cocos2dx/CCArmatureDisplay.h>
#include <dragonBones/cocos2dx/CCFactory.h>
#include <GameEngine/Player.h>
#include "cocos2d.h"

class DragonObject : public cocos2d::Sprite {

public:

    static DragonObject *create();

    dragonBones::CCArmatureDisplay *getDisplay();

    cocos2d::Node *getNodeByName(std::string name);

    float getGlobalHeight(std::string name);

    void addDOChild(cocos2d::Node* target);

    virtual void update();

    DragonObject();

protected:
    dragonBones::Armature *_armature;
    dragonBones::CCArmatureDisplay *_armatureDisplay;
    float _x_pos;
    float _y_pos;

    virtual void _updateAnimation();
};

class HeroPreview : public DragonObject {
public:

    HeroPreview();

    void changeHat(int id);
    void changeBow(int id);
    void changeArrow(int id);

    PlayerView* getPlayerView();
    dragonBones::Armature* getShoulders();
    void playAnimation(std::string name);

    float getHatHeight();

    cocos2d::Vec2 getSlotPosition(std::string name);
protected:

    PlayerView* _playerView;
    void _setPlayerView();

    dragonBones::CCArmatureDisplay *_bowArmatureDisplay;
    dragonBones::CCArmatureDisplay *_shouldersDisplay;
    dragonBones::Armature *_arrow;
    dragonBones::Armature *_shoulders;
    dragonBones::Armature *_bow;
    dragonBones::Armature *_hat;

    cocos2d::Node *_string;

    void _updateString();

    void _changeArrow();

    void _changeHat();

    void _changeBow();

    void _changeArrow(std::string path);

    void _changeHat(std::string path);

    void _changeBow(std::string path);

};


#endif //ARCUNLIM_DRAGONOBJECT_H
