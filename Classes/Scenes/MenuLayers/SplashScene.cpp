//
// Created by igor on 13.12.16.
//
#include <Localization/LocalizedStrings.h>
#include "MainMenu.h"
#include "SplashScene.h"

USING_NS_CC;

Scene *SplashScene::createScene() {

    auto scene = Scene::create();

    auto layer = SplashScene::create();

    scene->addChild(layer);

    return scene;
}


bool SplashScene::init() {
    if (!LayerColor::initWithColor(Color4B(255,95,0,255))) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    auto backgroundSprite = Sprite::create("splash.png");
    backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    this->addChild(backgroundSprite);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("env.plist");


    LanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();
    std::string language;

    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    switch (currentLanguageType) {
        case LanguageType::ENGLISH : {
            language = "ENGLISH";
            break;
        }
        case LanguageType::GERMAN : {
            language = "GERMAN";
            break;
        }
        case LanguageType::FRENCH : {
            language = "FRENCH";
            break;
        }
        case LanguageType::ITALIAN : {
            language = "ITALIAN";
            break;
        }
        case LanguageType::SPANISH : {
            language = "SPANISH";
            break;
        }
        case LanguageType::PORTUGUESE : {
            language = "PORTUGUESE";
            break;
        }
        case LanguageType::RUSSIAN : {
            language = "RUSSIAN";
            break;
        }
        case LanguageType::JAPANESE : {
            language = "JAPANESE";
            break;
        }
        case LanguageType::CHINESE : {
            language = "CHINESE";
            break;
        }
        default:{
            language = "ENGLISH";
            break;
        }
    }

    if(def->getStringForKey("LANGUAGE", "").length() == 0){
        def->setStringForKey("LANGUAGE", language.c_str());
    } else {
        language = def->getStringForKey("LANGUAGE", "");
    }

    LocalizedStrings::getInstance()->setLanguage(language);

    this->scheduleOnce(schedule_selector(SplashScene::GoToMainMenuScene), 2.f);

    return true;
}

void SplashScene::GoToMainMenuScene(float dt) {
    auto scene = MainScene::create();

    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}