//
// Created by igor on 10.10.16.
//

#ifndef ARCUNLIM_UI_H
#define ARCUNLIM_UI_H


#include "cocos2d.h"
#include <GameEngine/Objects/Hero.h>

class UI : public cocos2d::LayerColor {
public:
    bool init();

    CREATE_FUNC(UI);

    void initDuelBot(cocos2d::Size visibleSize, Hero *player1, Hero *player2);

    void initDuel(cocos2d::Size visibleSize, Hero *player1, Hero *player2);

    void initAppleMultiPlayer(cocos2d::Size visibleSize, Hero *player1, Hero *player2);

    void initApple(cocos2d::Size visibleSize, Hero *player);

    void initBattle(cocos2d::Size visibleSize, Hero *player);

    bool checkTouch(cocos2d::Vec2 touch);

    static void enableArrows(Hero *player, bool enable);

    void addMoveArrows(Hero *player);

    void setWarning(const char * message, cocos2d::Color3B color);

    void startGame();

   // void initTest(cocos2d::Size visibleSize);

protected:

    cocos2d::Label* _warningLabel;
    std::vector<cocos2d::Rect> _bounds;

};


#endif //ARCUNLIM_UI_H
