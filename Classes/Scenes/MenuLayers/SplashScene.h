//
// Created by igor on 13.12.16.
//

#ifndef ARCHERSUNLIMITED_SPLASHSCENE_H
#define ARCHERSUNLIMITED_SPLASHSCENE_H

#include "cocos2d.h"

class SplashScene : public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(SplashScene);

private:
    void GoToMainMenuScene( float dt );

};


#endif //ARCHERSUNLIMITED_SPLASHSCENE_H
