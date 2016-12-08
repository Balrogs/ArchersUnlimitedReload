#ifndef __BATTLE_H__
#define __BATTLE_H__

#include <GameEngine/Objects/Arrows/Arrow.h>
#include <GameEngine/Objects/Hero.h>
#include <GameEngine/Objects/Stickman.h>
#include <GameEngine/Objects/Aim/Aim.h>
#include <GameEngine/Objects/Brains/Brain.h>
#include <GameEngine/Global/Misc/SocketClient.h>
#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "Scenes/Layers/UI.h"
#include "Scenes/Layers/BackgroundLayer.h"

class BattleScene : public cocos2d::LayerColor {
public:
    static cocos2d::Scene *createScene(int type);

    virtual bool init();

    CREATE_FUNC(BattleScene);

    static const float MAX_ARROW_POWER;
    static const float MIN_ARROW_POWER;
    float GROUND;
    static const float G;
    static BattleScene *instance;

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    dragonBones::CCFactory factory;

    cocos2d::Node *getBulletPull();

    void addBrain(Brain *brain);

    void removeBrain(Brain *brain);

    void addTarget(cocos2d::Node *target);

    void removeTarget(cocos2d::Node *target);

    void addStickman();

    int getStickmanCount();

    float getGlobalScale();

    void moveScene(float x);

    bool isPaused() const;

    cocos2d::Vec2 getHeroPos(Hero *player);

    Hero* getPlayer();

    virtual void pauseBattle();

    virtual void unPause();

    virtual void onPopScene();

    cocos2d::Vec2 getPlayerPos();

    void showPopUp();

protected:
    Hero *_player;
    int _stickmanCount;
    float _GLOBAL_SCALE;
    UI *_ui;
    BackgroundLayer *_bg;
    bool _isPaused;

    std::vector<Brain *> _brains;
    std::vector<cocos2d::Node *> _targets;
    cocos2d::Node *_bullet_pull;


    virtual void initWorld();

    virtual void _enterFrameHandler(float passedTime);

    virtual void _keyBoardPressedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

    virtual void _keyBoardReleasedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

    virtual bool onContactBegin(cocos2d::PhysicsContact &contact);

    virtual bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event);

    virtual bool _touchHandlerMove(const cocos2d::Touch *touch, cocos2d::Event *event);

    virtual bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event);

    virtual bool isGameOver();

    void _pauseRecursive(cocos2d::Node *_node, bool _pause);


};

#endif // __BATTLE_H__