
#include <Scenes/PlayLayers/Battle.h>
#include <GameEngine/Global/WeaponSelector.h>
#include <ui/UIButton.h>
#include <GameEngine/Global/Variables.h>

USING_NS_CC;

bool UI::init() {
    if (!cocos2d::LayerColor::init()) {
        return false;
    }
    auto pause = cocos2d::ui::Button::create();
    pause->loadTextures(Variables::PAUSE_BUTTON, Variables::PAUSE_PRESSED_BUTTON, Variables::PAUSE_BUTTON,
                        cocos2d::ui::Widget::TextureResType::PLIST);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    pause->setPosition(
            Vec2(visibleSize.width / 2, visibleSize.height - pause->getBoundingBox().size.height / 2 - 25.f));
    pause->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                BattleScene::getInstance()->showPopUp();
                break;
            default:
                break;
        }
    });

    this->bounds.push_back(pause->getBoundingBox());

    this->addChild(pause);

    _warningLabel = cocos2d::Label::createWithTTF("", Variables::FONT_NAME, 32.f);
    _warningLabel->setColor(Color3B::BLACK);
    _warningLabel->setPosition(
            Vec2(pause->getPosition().x, pause->getPosition().y - pause->getBoundingBox().size.height));
    this->bounds.push_back(_warningLabel->getBoundingBox());
    this->addChild(_warningLabel);

    return true;
}

void UI::initBattle(Size visibleSize, Hero *player) {

    auto selector = new WeaponSelector(player);
    this->bounds.push_back(selector->getBoundingBox());
    this->addChild(selector);
}

void UI::initDuel(Size visibleSize, Hero *player1, Hero *player2) {

    auto player1_view = player1->getPlayer();
    player1_view->setPosition(10.f, 0.f);
    this->addChild(player1_view);
    this->bounds.push_back(player1_view->getBoundingBox());

    auto player2_view = player2->getPlayer();
    player2_view->setPosition(visibleSize.width / 2 + 60.f, 0.f);
    this->addChild(player2_view);
    this->bounds.push_back(player2_view->getBoundingBox());

    addMoveArrows(player1);
    addMoveArrows(player2);
}


void UI::initApple(Size visibleSize, Hero *player) {
    _warningLabel->setString("SHOOT APPLE");
    auto player1_view = player->getPlayer();
    player1_view->setPosition(10.f, 0.f);
    this->addChild(player1_view);
    this->bounds.push_back(player1_view->getBoundingBox());

}

bool UI::checkTouch(cocos2d::Vec2 touch) {
    for (auto bound : bounds) {
        if (bound.containsPoint(touch))
            return true;
    }
    return false;
}


void UI::enableArrows(Hero *player, bool enable) {
    if (cocos2d::ui::Button *left_arrow = dynamic_cast<cocos2d::ui::Button *>(player->getChildByName("left arrow"))) {
        left_arrow->setEnabled(enable);
        left_arrow->setVisible(enable);
    }
    if (cocos2d::ui::Button *right_arrow = dynamic_cast<cocos2d::ui::Button *>(player->getChildByName("right arrow"))) {
        right_arrow->setEnabled(enable);
        right_arrow->setVisible(enable);
    }
}

void UI::addMoveArrows(Hero *player) {
    auto left_arrow = cocos2d::ui::Button::create();
    left_arrow->loadTextures(Variables::MOVE_BUTTON, Variables::MOVE_PRESSED_BUTTON, Variables::MOVE_BUTTON,
                             cocos2d::ui::Widget::TextureResType::PLIST);

    auto right_arrow = cocos2d::ui::Button::create();
    right_arrow->loadTextures(Variables::MOVE_BUTTON, Variables::MOVE_PRESSED_BUTTON, Variables::MOVE_BUTTON,
                              cocos2d::ui::Widget::TextureResType::PLIST);


    left_arrow->setScaleX(-1);
    left_arrow->setPosition(cocos2d::Vec2(-150.f, 100.f));
    right_arrow->setPosition(cocos2d::Vec2(150.f, 100.f));

    right_arrow->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                BattleScene::getInstance()->getPlayer()->move(1);
                enableArrows(BattleScene::getInstance()->getPlayer(), false);
                break;
            default:
                break;
        }
    });

    left_arrow->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                BattleScene::getInstance()->getPlayer()->move(-1);
                enableArrows(BattleScene::getInstance()->getPlayer(), false);
                break;
            default:
                break;
        }
    });


    this->bounds.push_back(left_arrow->getBoundingBox());
    this->bounds.push_back(right_arrow->getBoundingBox());

    player->addChild(left_arrow, 1, "left arrow");
    player->addChild(right_arrow, 1, "right arrow");
}

void UI::setWarning(const char *message, Color3B color) {
    _warningLabel->setColor(color);
    _warningLabel->setString(message);
}
//
//void UI::initTest(cocos2d::Size visibleSize) {
//
//    auto maxspeed_label = cocos2d::Label::createWithTTF("MAX SPEED", Variables::FONT_NAME, 32.f);
//    maxspeed_label->setColor(Color3B::BLACK);
//    maxspeed_label->setPosition(
//            Vec2(visibleSize.width / 4, visibleSize.height - maxspeed_label->getBoundingBox().size.height / 2 - 25.f));
//    this->bounds.push_back(maxspeed_label->getBoundingBox());
//    this->addChild(maxspeed_label);
//
//    auto maxspeed_value = cocos2d::Label::createWithTTF(StringUtils::toString(BattleScene::MAX_ARROW_POWER), Variables::FONT_NAME, 32.f);
//    maxspeed_value->setColor(Color3B::BLACK);
//    maxspeed_value->setPosition(
//            Vec2(maxspeed_label->getPosition().x, maxspeed_label->getPosition().y - 40.f));
//    this->bounds.push_back(maxspeed_value->getBoundingBox());
//    this->addChild(maxspeed_value);
//
//    auto arrow_maxspeed_up = cocos2d::ui::Button::create();
//    arrow_maxspeed_up->loadTextures(Variables::DROP_LIST_BUTTON, Variables::DROP_LIST_PRESSED_BUTTON,
//                                    Variables::DROP_LIST_BUTTON,
//                                    cocos2d::ui::Widget::TextureResType::PLIST);
//    arrow_maxspeed_up->setPosition(
//            Vec2(maxspeed_value->getPosition().x + 100.f,
//                 maxspeed_value->getPosition().y));
//    arrow_maxspeed_up->addTouchEventListener(
//            [&, maxspeed_value](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
//                switch (type) {
//                    case cocos2d::ui::Widget::TouchEventType::ENDED:
//                        BattleScene::MAX_ARROW_POWER += 1.f;
//                        maxspeed_value->setString(StringUtils::toString(BattleScene::MAX_ARROW_POWER));
//                        break;
//                    default:
//                        break;
//                }
//            });
//
//    this->bounds.push_back(arrow_maxspeed_up->getBoundingBox());
//
//    this->addChild(arrow_maxspeed_up);
//
//    auto arrow_maxspeed_down = cocos2d::ui::Button::create();
//    arrow_maxspeed_down->loadTextures(Variables::DROP_LIST_BUTTON, Variables::DROP_LIST_PRESSED_BUTTON,
//                                      Variables::DROP_LIST_BUTTON,
//                                      cocos2d::ui::Widget::TextureResType::PLIST);
//    arrow_maxspeed_down->setPosition(
//            Vec2(maxspeed_value->getPosition().x - 100.f,
//                 maxspeed_value->getPosition().y));
//    arrow_maxspeed_down->addTouchEventListener([&, maxspeed_value](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
//        switch (type) {
//            case cocos2d::ui::Widget::TouchEventType::ENDED:
//                BattleScene::MAX_ARROW_POWER -= 1.f;
//                maxspeed_value->setString(StringUtils::toString(BattleScene::MAX_ARROW_POWER));
//                break;
//            default:
//                break;
//        }
//    });
//
//    this->bounds.push_back(arrow_maxspeed_down->getBoundingBox());
//
//    this->addChild(arrow_maxspeed_down);
//
//    auto minspeed_label = cocos2d::Label::createWithTTF("MIN SPEED", Variables::FONT_NAME, 32.f);
//    minspeed_label->setColor(Color3B::BLACK);
//    minspeed_label->setPosition(
//            Vec2(maxspeed_label->getPosition().x, maxspeed_label->getPosition().y - 100.f));
//    this->bounds.push_back(minspeed_label->getBoundingBox());
//    this->addChild(minspeed_label);
//
//    auto minspeed_value = cocos2d::Label::createWithTTF(StringUtils::toString(BattleScene::MIN_ARROW_POWER), Variables::FONT_NAME, 32.f);
//    minspeed_value->setColor(Color3B::BLACK);
//    minspeed_value->setPosition(
//            Vec2(minspeed_label->getPosition().x, minspeed_label->getPosition().y - 40.f));
//    this->bounds.push_back(minspeed_value->getBoundingBox());
//    this->addChild(minspeed_value);
//
//    auto arrow_minspeed_up = cocos2d::ui::Button::create();
//    arrow_minspeed_up->loadTextures(Variables::DROP_LIST_BUTTON, Variables::DROP_LIST_PRESSED_BUTTON,
//                                    Variables::DROP_LIST_BUTTON,
//                                    cocos2d::ui::Widget::TextureResType::PLIST);
//    arrow_minspeed_up->setPosition(
//            Vec2(minspeed_value->getPosition().x + 100.f,
//                 minspeed_value->getPosition().y));
//    arrow_minspeed_up->addTouchEventListener([&,minspeed_value](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
//        switch (type) {
//            case cocos2d::ui::Widget::TouchEventType::ENDED:
//                BattleScene::MIN_ARROW_POWER += 1.f;
//                minspeed_value->setString(StringUtils::toString(BattleScene::MIN_ARROW_POWER));
//                break;
//            default:
//                break;
//        }
//    });
//
//    this->bounds.push_back(arrow_minspeed_up->getBoundingBox());
//
//    this->addChild(arrow_minspeed_up);
//
//    auto arrow_minspeed_down = cocos2d::ui::Button::create();
//    arrow_minspeed_down->loadTextures(Variables::DROP_LIST_BUTTON, Variables::DROP_LIST_PRESSED_BUTTON,
//                                      Variables::DROP_LIST_BUTTON,
//                                      cocos2d::ui::Widget::TextureResType::PLIST);
//    arrow_minspeed_down->setPosition(
//            Vec2(minspeed_value->getPosition().x - 100.f,
//                 minspeed_value->getPosition().y));
//    arrow_minspeed_down->addTouchEventListener([&,minspeed_value](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
//        switch (type) {
//            case cocos2d::ui::Widget::TouchEventType::ENDED:
//                BattleScene::MIN_ARROW_POWER -= 1.f;
//                minspeed_value->setString(StringUtils::toString(BattleScene::MIN_ARROW_POWER));
//                break;
//            default:
//                break;
//        }
//    });
//
//    this->bounds.push_back(arrow_minspeed_down->getBoundingBox());
//
//    this->addChild(arrow_minspeed_down);
//
//    auto update_label = cocos2d::Label::createWithTTF("UPDATE", Variables::FONT_NAME, 32.f);
//    update_label->setColor(Color3B::BLACK);
//    update_label->setPosition(
//            Vec2(minspeed_label->getPosition().x, minspeed_label->getPosition().y - 100.f));
//    this->bounds.push_back(update_label->getBoundingBox());
//    this->addChild(update_label);
//
//    auto update_value = cocos2d::Label::createWithTTF(StringUtils::toString(BattleScene::ARROW_UPDATE), Variables::FONT_NAME, 32.f);
//    update_value->setColor(Color3B::BLACK);
//    update_value->setPosition(
//            Vec2(update_label->getPosition().x, update_label->getPosition().y - 40.f));
//    this->bounds.push_back(update_value->getBoundingBox());
//    this->addChild(update_value);
//
//    auto arrow_update_up = cocos2d::ui::Button::create();
//    arrow_update_up->loadTextures(Variables::DROP_LIST_BUTTON, Variables::DROP_LIST_PRESSED_BUTTON,
//                                    Variables::DROP_LIST_BUTTON,
//                                    cocos2d::ui::Widget::TextureResType::PLIST);
//    arrow_update_up->setPosition(
//            Vec2(update_value->getPosition().x + 100.f,
//                 update_value->getPosition().y));
//    arrow_update_up->addTouchEventListener([&,update_value](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
//        switch (type) {
//            case cocos2d::ui::Widget::TouchEventType::ENDED:
//                BattleScene::ARROW_UPDATE += 0.01f;
//                update_value->setString(StringUtils::toString(BattleScene::ARROW_UPDATE));
//                break;
//            default:
//                break;
//        }
//    });
//
//    this->bounds.push_back(arrow_update_up->getBoundingBox());
//
//    this->addChild(arrow_update_up);
//
//    auto arrow_update_down = cocos2d::ui::Button::create();
//    arrow_update_down->loadTextures(Variables::DROP_LIST_BUTTON, Variables::DROP_LIST_PRESSED_BUTTON,
//                                      Variables::DROP_LIST_BUTTON,
//                                      cocos2d::ui::Widget::TextureResType::PLIST);
//    arrow_update_down->setPosition(
//            Vec2(update_value->getPosition().x - 100.f,
//                 update_value->getPosition().y));
//    arrow_update_down->addTouchEventListener([&,update_value](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
//        switch (type) {
//            case cocos2d::ui::Widget::TouchEventType::ENDED:
//                BattleScene::ARROW_UPDATE -= 0.01f;
//                update_value->setString(StringUtils::toString(BattleScene::ARROW_UPDATE));
//                break;
//            default:
//                break;
//        }
//    });
//
//    this->bounds.push_back(arrow_update_down->getBoundingBox());
//
//    this->addChild(arrow_update_down);
//    auto gravity_label = cocos2d::Label::createWithTTF("GRAVITY", Variables::FONT_NAME, 32.f);
//    gravity_label->setColor(Color3B::BLACK);
//    gravity_label->setPosition(
//            Vec2(update_label->getPosition().x, update_label->getPosition().y - 100.f));
//    this->bounds.push_back(gravity_label->getBoundingBox());
//    this->addChild(gravity_label);
//
//    auto gravity_value = cocos2d::Label::createWithTTF(StringUtils::toString(BattleScene::ARROW_UPDATE), Variables::FONT_NAME, 32.f);
//    gravity_value->setColor(Color3B::BLACK);
//    gravity_value->setPosition(
//            Vec2(gravity_label->getPosition().x, gravity_label->getPosition().y - 40.f));
//    this->bounds.push_back(gravity_value->getBoundingBox());
//    this->addChild(gravity_value);
//
//    auto arrow_gravity_up = cocos2d::ui::Button::create();
//    arrow_gravity_up->loadTextures(Variables::DROP_LIST_BUTTON, Variables::DROP_LIST_PRESSED_BUTTON,
//                                  Variables::DROP_LIST_BUTTON,
//                                  cocos2d::ui::Widget::TextureResType::PLIST);
//    arrow_gravity_up->setPosition(
//            Vec2(gravity_value->getPosition().x + 100.f,
//                 gravity_value->getPosition().y));
//    arrow_gravity_up->addTouchEventListener([&,gravity_value](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
//        switch (type) {
//            case cocos2d::ui::Widget::TouchEventType::ENDED:
//                BattleScene::G += 0.05f;
//                gravity_value->setString(StringUtils::toString(BattleScene::G));
//                break;
//            default:
//                break;
//        }
//    });
//
//    this->bounds.push_back(arrow_gravity_up->getBoundingBox());
//
//    this->addChild(arrow_gravity_up);
//
//    auto arrow_gravity_down = cocos2d::ui::Button::create();
//    arrow_gravity_down->loadTextures(Variables::DROP_LIST_BUTTON, Variables::DROP_LIST_PRESSED_BUTTON,
//                                    Variables::DROP_LIST_BUTTON,
//                                    cocos2d::ui::Widget::TextureResType::PLIST);
//    arrow_gravity_down->setPosition(
//            Vec2(gravity_value->getPosition().x - 100.f,
//                 gravity_value->getPosition().y));
//    arrow_gravity_down->addTouchEventListener([&,gravity_value](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
//        switch (type) {
//            case cocos2d::ui::Widget::TouchEventType::ENDED:
//                BattleScene::G -= 0.05f;
//                gravity_value->setString(StringUtils::toString(BattleScene::G));
//                break;
//            default:
//                break;
//        }
//    });
//
//    this->bounds.push_back(arrow_gravity_down->getBoundingBox());
//
//    this->addChild(arrow_gravity_down);
//
//}
