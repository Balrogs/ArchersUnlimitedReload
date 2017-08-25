#ifndef ARCUNLIM_DUELSCENE_H
#define ARCUNLIM_DUELSCENE_H

#include <GameEngine/Player.h>
#include <Scenes/PlayLayers/BattleParent.h>
#include <GameEngine/Objects/Hero.h>
#include <GameEngine/Brains/Brain.h>

class DuelScene : public BattleParent {
public:

    static DuelScene *create(Statistics *stats);

    virtual void makeTurn(int id);

    void moveScene(float x);

    virtual int getPlayerId();

    int getGainedCoinsByActionType(int type) override;

protected:

    int _turnId;

    Player *_player1;
    Player *_player2;

    Hero *_hero1;
    Hero *_hero2;

    bool _isStarted;

    void initWorld() override;

    bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) override;

    bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) override;

    bool isGameOver() override;

    void _startGame();

    void _setTurnId(int id);
private:
    Brain* _brain;


};


#endif //ARCUNLIM_DUELSCENE_H
