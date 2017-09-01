#include <GameEngine/Global/Variables.h>
#include <ui/UIButton.h>
#include <Localization/LocalizedStrings.h>
#include <Scenes/MenuLayers/Main/MainMenu.h>
#include <Scenes/MenuLayers/Main/Randomizer.h>
#include <GameEngine/Global/Misc/UI/ContinueButton.h>
#include "GameOverScene.h"
#include "Battle.h"

GameOverScene *GameOverScene::create(Statistics *stats) {
    GameOverScene *ret = new(std::nothrow) GameOverScene();
    if (ret && ret->init(stats)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GameOverScene::init(Statistics *stats) {
    if (!Layer::init()) {
        return false;
    }
    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto black = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(black);

    auto bg = Sprite::createWithSpriteFrameName(Variables::BG2);

    bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    bg->setScaleX((2 * visibleSize.width / 3) / bg->getContentSize().width);

    bg->setScaleY((visibleSize.height / 2) / bg->getContentSize().height);

    this->addChild(bg, 1);

    _title1 = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("GAME OVER"),
                                            Variables::FONT_NAME,
                                            Variables::H_FONT_SIZE());
    _title1->setPosition(visibleSize.width / 2,
                         bg->getBoundingBox().getMaxY() - _title1->getContentSize().height / 2 - 50.f);
    this->addChild(_title1, 2);


    _title2 = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("GAME OVER"),
                                            Variables::FONT_NAME,
                                            Variables::H_FONT_SIZE());
    _title2->setPosition(_title1->getPosition().x - 10.f, _title1->getPosition().y + 10.f);
    _title2->setColor(Color3B::RED);
    this->addChild(_title2, 2);

    this->schedule(SEL_SCHEDULE(&GameOverScene::update), 0.7f);

    const auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event *event) {
        switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_BREAK:
            case EventKeyboard::KeyCode::KEY_ESCAPE:
            case EventKeyboard::KeyCode::KEY_BACKSPACE: {
                onQuit();
            }
                break;
            default:
                break;
        }
    };

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    auto repeatButton = ui::Button::create();
    repeatButton->loadTextures(Variables::AGAIN_BUTTON, Variables::AGAIN_PRESSED_BUTTON,
                               Variables::AGAIN_BUTTON, ui::Widget::TextureResType::PLIST);
    repeatButton->setScale(1.5f);
    repeatButton->addTouchEventListener([&, stats](Ref *sender, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::ENDED: {
                _saveStats(stats);
                Director::getInstance()->replaceScene(BattleScene::createScene(stats->getType()));
                break;
            }
            default:
                break;
        }
    });



    auto backButton = ui::Button::create();
    backButton->loadTextures(Variables::CLOSE_BUTTON_PATH, Variables::CLOSE_PRESSED_BUTTON_PATH,
                             Variables::CLOSE_BUTTON_PATH, ui::Widget::TextureResType::PLIST);

    backButton->addTouchEventListener([&, stats](Ref *sender, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::ENDED: {
                _saveStats(stats);
                onQuit();
                break;
            }
            default:
                break;
        }
    });

    auto coins = def->getIntegerForKey("COINS", 0);

    if (coins >= Variables::continueCost() && (stats->getType() < 2 || (stats->getType() == 2 && !stats->isVictory()))) {

        repeatButton->setPosition(Vec2(visibleSize.width / 2,
                                       bg->getBoundingBox().getMinY()));
        backButton->setPosition(Vec2(repeatButton->getPosition().x - repeatButton->getBoundingBox().size.width / 2 -
                                     backButton->getBoundingBox().size.width,
                                     repeatButton->getPosition().y));

        auto continueButton = ContinueButton::create();

        continueButton->button()->addTouchEventListener([def, coins, stats](Ref *sender, ui::Widget::TouchEventType type) {
            switch (type) {
                case ui::Widget::TouchEventType::ENDED: {
                    def->setIntegerForKey("COINS", coins - Variables::continueCost());
                    Director::getInstance()->replaceScene(BattleScene::createScene(stats));
                }
                    break;
                default:
                    break;
            }
        });

        continueButton->setPosition(Vec2(repeatButton->getPosition().x + repeatButton->getBoundingBox().size.width / 2 +
                                         continueButton->getBoundingBox().width,
                                         repeatButton->getPosition().y));
        continueButton->enable();

        this->addChild(continueButton, 3);
    } else {
        repeatButton->setPosition(Vec2(visibleSize.width / 2 + repeatButton->getBoundingBox().size.width,
                                       bg->getBoundingBox().getMinY()));

        backButton->setScale(1.5f);
        backButton->setPosition(Vec2(visibleSize.width / 2 - backButton->getBoundingBox().size.width,
                                     repeatButton->getPosition().y));
    }


    this->addChild(repeatButton, 3);
    this->addChild(backButton, 3);

    auto stats_size = Size(
            bg->getContentSize().width,
            _title1->getBoundingBox().getMinY() - repeatButton->getBoundingBox().getMaxY()
    );
    auto stats_view = stats->getView(stats_size);

    stats_view->setPosition(Vec2(
            bg->getBoundingBox().getMinX() +  stats_size.width / 2,
            bg->getPositionY() - stats_size.height / 2
    ));
    this->addChild(stats_view, 2);


    auto coins_bar = cocos2d::ui::Button::create();
    coins_bar->loadTextureNormal(Variables::COIN_BAR, cocos2d::ui::Widget::TextureResType::PLIST);

    coins_bar->addTouchEventListener([&, stats, coins](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                if(coins >= Variables::wheelCost()){
                    _saveStats(stats);
                    onQuit();
                    MainScene::getInstance()->replaceMain(Randomizer::create());
                }
            }
                break;
            default:
                break;
        }
    });
    coins_bar->setScale(0.5f);
    coins_bar->setPosition(Vec2(visibleSize.width - coins_bar->getBoundingBox().size.width / 2 - 15.f,
                                visibleSize.height - coins_bar->getBoundingBox().size.height / 2 - 15.f));


    auto coinsCount = cocos2d::Label::createWithTTF(StringUtils::toString(coins), Variables::FONT_NAME,
                                                Variables::FONT_SIZE(), Size(3 * coins_bar->getContentSize().width / 5, Variables::FONT_SIZE()));
    coinsCount->setHorizontalAlignment(TextHAlignment::RIGHT);
    coinsCount->setColor(Color3B::WHITE);
    coinsCount->setAnchorPoint(Vec2(0, 0));
    coinsCount->setPosition(15.f, coins_bar->getContentSize().height / 2 - Variables::FONT_SIZE() / 2);

    coins_bar->addChild(coinsCount, 5);

    this->addChild(coins_bar);

    return true;
}

void GameOverScene::onQuit() {
    Director::getInstance()->popScene();
}

void GameOverScene::update(float dt) {
    Node::update(dt);
    auto tmpColor = _title1->getColor();
    _title1->setColor(_title2->getColor());
    _title2->setColor(tmpColor);
}

void GameOverScene::_saveStats(Statistics *stats) {
    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();
    auto coins = def->getIntegerForKey("COINS", 0);
    auto battles = def->getIntegerForKey("BATTLES_COUNT", 0);
    def->setIntegerForKey("COINS", coins + stats->getCoinsGained());
    def->setIntegerForKey("BATTLES_COUNT", battles + 1);
}

void GameOverScene::_loadButtons() {

}

void MultiPlayerGameOverScene::_loadButtons() {

}
