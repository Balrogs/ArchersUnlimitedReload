
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
                BattleScene::instance->showPopUp();
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
    player2_view->setPosition(BattleScene::instance->visibleSize.width / 2 + 60.f, 0.f);
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
                BattleScene::instance->getPlayer()->move(1);
                enableArrows(BattleScene::instance->getPlayer(), false);
                break;
            default:
                break;
        }
    });

    left_arrow->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                BattleScene::instance->getPlayer()->move(-1);
                enableArrows(BattleScene::instance->getPlayer(), false);
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
