#ifndef ARCHERSUNLIMITED_BATTLEPARENT_H
#define ARCHERSUNLIMITED_BATTLEPARENT_H

#include "dragonBones/DragonBonesHeaders.h"
#include <Scenes/Layers/BackgroundLayer.h>
#include <Scenes/Layers/UI.h>
#include <GameEngine/Objects/Hero.h>
#include <GameEngine/Statistics.h>
#include <GameEngine/Global/Misc/SocketClient.h>
#include "cocos2d.h"

class BattleParent : public cocos2d::LayerColor {
public:

    static BattleParent* getInstance();

    virtual bool init(Statistics *stats);

    static cocos2d::Scene *createScene(int type);
    static cocos2d::Scene *createScene(Statistics* stats);

    static const float MAX_ARROW_POWER;
    static const float MIN_ARROW_POWER;
    static float GROUND;
    static const float G;

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    dragonBones::CCFactory factory;

    cocos2d::Node *getBulletPull();

    void addStickman();

    int getStickmanCount();

    float getGlobalScale();

    bool isPaused() const;

    BackgroundLayer *getBackground();

    cocos2d::Vec2 getHeroPos(Hero *player);

    Hero *getPlayer();

    virtual void pauseBattle();

    virtual void unPause();

    virtual void onPopScene();

    cocos2d::Vec2 getPlayerPos();

    void showPopUp();

    virtual void addCoins(int value);

protected:

    static BattleParent *_instance;

    //layers
    UI *_ui;
    BackgroundLayer *_bg;

    //disable multitouch variable
    int _touch;

    //initial vars
    float _GLOBAL_SCALE;
    bool _isPaused;

    // battle info
    Statistics *_stats;
    SocketClient *_client;

    //children refs
    Hero *_player;
    int _stickmanCount;

    cocos2d::Node *_bullet_pull;

    virtual void initWorld() = 0;

    virtual void _enterFrameHandler(float passedTime);

    virtual void _keyBoardPressedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

    virtual void _keyBoardReleasedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

    virtual bool onContactBegin(cocos2d::PhysicsContact &contact);

    virtual bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event);

    virtual bool _touchHandlerMove(const cocos2d::Touch *touch, cocos2d::Event *event);

    virtual bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event);

    virtual bool isGameOver();

    virtual void _gameOver();

    virtual int _getGainedCoinsByActionType(int type) = 0;

    void _pauseRecursive(cocos2d::Node *_node, bool _pause);

    void _createEnvForStickman(Body *stickman, int type);
};

#endif //ARCHERSUNLIMITED_BATTLEPARENT_H
