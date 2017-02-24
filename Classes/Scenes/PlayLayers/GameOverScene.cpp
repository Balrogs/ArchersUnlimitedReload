#include <GameEngine/Global/Variables.h>
#include <ui/UIButton.h>
#include <Localization/LocalizedStrings.h>
#include <Scenes/MenuLayers/MainMenu.h>
#include <Scenes/MenuLayers/Randomizer.h>
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

    this->addChild(bg, 1);

    _title1 = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("GAME OVER"),
                                            Variables::FONT_NAME,
                                            Variables::H_FONT_SIZE);
    _title1->setPosition(visibleSize.width / 2,
                         bg->getBoundingBox().getMaxY() - _title1->getContentSize().height / 2 - 50.f);
    this->addChild(_title1, 2);


    _title2 = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("GAME OVER"),
                                            Variables::FONT_NAME,
                                            Variables::H_FONT_SIZE);
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
    repeatButton->setPosition(Vec2(visibleSize.width / 2,
                                   repeatButton->getBoundingBox().size.height / 2 + bg->getBoundingBox().getMinY() +
                                   25.f));
    this->addChild(repeatButton, 3);

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
    backButton->setPosition(Vec2(repeatButton->getPosition().x - repeatButton->getBoundingBox().size.width / 2 -
                                 backButton->getBoundingBox().size.width,
                                 repeatButton->getPosition().y));
    this->addChild(backButton, 3);

    if (stats->getType() < 3) {
        auto continueButton = ui::Button::create();
        continueButton->loadTextures(Variables::CONTINUE_BUTTON, Variables::CONTINUE_PRESSED_BUTTON,
                                     Variables::CONTINUE_BUTTON, ui::Widget::TextureResType::PLIST);

        continueButton->addTouchEventListener([&, stats](Ref *sender, ui::Widget::TouchEventType type) {
            switch (type) {
                case ui::Widget::TouchEventType::ENDED: {
                    // TODO show media
                    Director::getInstance()->replaceScene(BattleScene::createScene(stats));
                }
                    break;
                default:
                    break;
            }
        });
        continueButton->setPosition(Vec2(repeatButton->getPosition().x + repeatButton->getBoundingBox().size.width / 2 +
                                         continueButton->getBoundingBox().size.width,
                                         repeatButton->getPosition().y));

        this->addChild(continueButton, 3);
    }

    auto coins = def->getIntegerForKey("COINS", 0);

    auto chestButton = ui::Button::create();
    chestButton->loadTextures(Variables::CHEST_BUTTON, Variables::CHEST_PRESSED_BUTTON,
                              Variables::CHEST_BUTTON, ui::Widget::TextureResType::PLIST);

    chestButton->addTouchEventListener([&, stats, def, coins](Ref *sender, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::ENDED: {
                if (coins + stats->getCoinsGained() >= 1000) {
                    def->setIntegerForKey("COINS", coins - 1000);
                    def->destroyInstance();
                    _saveStats(stats);
                    onQuit();
                    MainScene::getInstance()->replaceMain(Randomizer::create());
                } else {
                    //TODO show media
                }
            }
                break;
            default:
                break;
        }
    });
    chestButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    this->addChild(chestButton, 3);

    auto stats_size = Size(
            bg->getContentSize().width / 2,
            _title1->getPosition().y - chestButton->getPosition().y + chestButton->getBoundingBox().size.height / 2
    );
    auto stats_view = stats->getView(stats_size);

    stats_view->setPosition(Vec2(
            bg->getBoundingBox().getMinX() +  stats_size.width,
            chestButton->getPosition().y + chestButton->getBoundingBox().size.height / 2
    ));
    this->addChild(stats_view, 2);

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
