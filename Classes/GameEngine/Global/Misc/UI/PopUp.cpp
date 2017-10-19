#include <GameEngine/Global/Variables.h>
#include <Scenes/PlayLayers/Battle.h>
#include <ui/UIButton.h>
#include <Scenes/MenuLayers/Main/MainMenu.h>
#include <Scenes/MenuLayers/Multiplayer/Lobby.h>
#include <Localization/LocalizedStrings.h>
#include <Scenes/PlayLayers/MultiplayerBattle.h>
#include "PopUp.h"

USING_NS_CC;

PopUp *PopUp::create(std::string title, cocos2d::Node *message, bool isTwoButtons) {
    PopUp *ret = new(std::nothrow) PopUp();
    if (ret && ret->init(title, message, isTwoButtons)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

PopUp *PopUp::create(std::string title, cocos2d::Node *message) {
    PopUp *ret = new(std::nothrow) PopUp();
    if (ret && ret->init(title, message)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool PopUp::init(std::string title) {
    if (!Node::init()) {
        return false;
    }
    _visibleSize = Director::getInstance()->getVisibleSize();
    POPUP_SIZE = Size(_visibleSize.width / 2, _visibleSize.height - 100.f);


    auto black = LayerColor::create(Color4B(0, 0, 0, 160), _visibleSize.width, _visibleSize.height * 4);
    auto pos = Vec2(-_visibleSize.width / 2,
                    - 2 *_visibleSize.height);
    black->setPosition(pos);
    this->addChild(black);

    auto base = Sprite::createWithSpriteFrameName(Variables::BG2);
    _scale = Vec2(POPUP_SIZE.width / base->getContentSize().width, POPUP_SIZE.height / base->getContentSize().height);
    base->setScale(_scale.x, _scale.y);
    this->addChild(base);

    _title = Label::createWithTTF(title, Variables::FONT_NAME, Variables::H_FONT_SIZE());
    _title->setPosition(0.f, POPUP_SIZE.height / 2 - _title->getContentSize().height / 2 - 20.f);
    _title->setColor(cocos2d::Color3B::BLACK);
    this->addChild(_title, 2);

    return true;
}

bool PopUp::init(std::string title, cocos2d::Node *message) {
    if (!PopUp::init(title)) {
        return false;
    }
    message->setPosition(0, 0);
    this->addChild(message, 2);
    return true;
}

bool PopUp::init(std::string title, cocos2d::Node *message, bool isTwoButtons) {
    if (!PopUp::init(title)) {
        return false;
    }

    message->setPosition(0, 0);
    this->addChild(message, 2);

    if (isTwoButtons) {
        auto yes = ui::Button::create();
        yes->loadTextures(Variables::YES_BUTTON_PATH, Variables::YES_PRESSED_BUTTON_PATH, Variables::YES_BUTTON_PATH,
                          cocos2d::ui::Widget::TextureResType::PLIST);
        yes->setPosition(Vec2(-yes->getBoundingBox().size.width, 0.f));

        yes->addTouchEventListener(CC_CALLBACK_0(PopUp::yesAction, this));

        auto no = ui::Button::create();
        no->loadTextures(Variables::NO_BUTTON_PATH, Variables::NO_PRESSED_BUTTON_PATH, Variables::NO_BUTTON_PATH,
                         cocos2d::ui::Widget::TextureResType::PLIST);
        no->setPosition(Vec2(no->getBoundingBox().size.width, 0.f));
        no->addTouchEventListener(CC_CALLBACK_0(PopUp::noAction, this));

        _buttons = Node::create();
        _buttons->addChild(yes);
        _buttons->addChild(no);
        _buttons->setPosition(0.f, -POPUP_SIZE.height / 2 + yes->getBoundingBox().size.height / 2 + 25.f);

    } else {
        auto ok = ui::Button::create();
        ok->loadTextures(Variables::YES_BUTTON_PATH, Variables::YES_PRESSED_BUTTON_PATH, Variables::YES_BUTTON_PATH,
                         cocos2d::ui::Widget::TextureResType::PLIST);
        ok->addTouchEventListener(CC_CALLBACK_0(PopUp::okAction, this));

        _buttons = Node::create();
        _buttons->addChild(ok);
        _buttons->setPosition(0, -POPUP_SIZE.height / 2 + ok->getBoundingBox().size.height / 2 + 25.f);
    }

    this->addChild(_buttons, 2);
    return true;
}

void PopUp::noAction() {
    this->removeFromParent();
    BattleParent::getInstance()->unPause();
}

void PopUp::yesAction() {
    this->removeFromParent();
    BattleParent::getInstance()->onPopScene();
}

void PopUp::okAction() {
    this->removeFromParent();
    BattleParent::getInstance()->onPopScene();
}

void PopUp::onEnterTransitionDidFinish() {
    Node::onEnterTransitionDidFinish();
    this->runAction(MoveTo::create(.5f, Vec2(_visibleSize.width / 2, _visibleSize.height / 2)));
}

void PopUp::removeFromParentAndCleanup(bool cleanup) {
    this->runAction(Sequence::create(
            MoveTo::create(.3f, Vec2(_visibleSize.width / 2, 1.5f * _visibleSize.height)),
            CallFunc::create([&](){
                Node::removeFromParentAndCleanup(cleanup);
            }),
            NULL)
    );
}

void MainMenuPopUp::noAction() {
    this->getParent()->getEventDispatcher()->resumeEventListenersForTarget(this->getParent(), true);
    this->removeFromParent();
}

void MainMenuPopUp::yesAction() {
    this->removeFromParent();
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

}

void MainMenuPopUp::okAction() {
    this->getParent()->getEventDispatcher()->resumeEventListenersForTarget(this->getParent(), true);
    this->removeFromParent();
}

MainMenuPopUp *MainMenuPopUp::create(std::string title, cocos2d::Node *message, bool isTwoButtons) {
    MainMenuPopUp *ret = new(std::nothrow) MainMenuPopUp();
    if (ret && ret->init(title, message, isTwoButtons)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

MainMenuPopUp *MainMenuPopUp::create(std::string title, cocos2d::Node *message) {
    MainMenuPopUp *ret = new(std::nothrow) MainMenuPopUp();
    if (ret && ret->init(title, message)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

PausePopUp *PausePopUp::create(std::string title) {
    PausePopUp *ret = new(std::nothrow) PausePopUp();
    if (ret && ret->init(title)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool PausePopUp::init(std::string title) {
    if (!PopUp::init(title)) {
        return false;
    }

    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    _musicState = def->getBoolForKey("MUSIC", true);
    _effectsState = def->getBoolForKey("EFFECTS", true);

    def->flush();

    _reloadButtons();

    auto goToMenu = ui::Button::create();
    goToMenu->loadTextures(Variables::RED_BUTTON, Variables::RED_PRESSED_BUTTON, Variables::RED_BUTTON,
                           cocos2d::ui::Widget::TextureResType::PLIST);
    goToMenu->setPosition(Vec2(0.f, 0.f));

    goToMenu->addTouchEventListener(CC_CALLBACK_0(PausePopUp::yesAction, this));

    auto goToMenuTitle = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("GO TO MENU"), Variables::FONT_NAME,
                                                       Variables::FONT_SIZE());
    goToMenuTitle->setPosition(goToMenu->getContentSize().width / 2,
                               goToMenu->getContentSize().height / 2);
    goToMenu->addChild(goToMenuTitle, 4);

    auto play = ui::Button::create();
    play->loadTextures(Variables::BLUE_BUTTON, Variables::BLUE_PRESSED_BUTTON, Variables::BLUE_BUTTON,
                       cocos2d::ui::Widget::TextureResType::PLIST);
    play->setPosition(Vec2(0.f, goToMenu->getPosition().y - 3 * goToMenu->getBoundingBox().size.height / 2));
    play->addTouchEventListener(CC_CALLBACK_0(PausePopUp::noAction, this));

    auto playTitle = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("CONTINUE"), Variables::FONT_NAME,
                                                   Variables::FONT_SIZE());
    playTitle->setPosition(play->getContentSize().width / 2,
                           play->getContentSize().height / 2);
    play->addChild(playTitle, 4);

    _buttons = Node::create();
    _buttons->addChild(goToMenu);
    _buttons->addChild(play);
    _buttons->setPosition(0, - goToMenu->getContentSize().height / 2);
    this->addChild(_buttons, 2);

    return true;
}

void PausePopUp::_reloadButtons() {
    if (_musicButton != nullptr) {
        _musicButton->removeFromParent();
    }

    if (_effectsButton != nullptr) {
        _effectsButton->removeFromParent();
    }

    string music = Variables::MUSIC_ON_BUTTON;
    string effects = Variables::EFFECTS_ON_BUTTON;

    if (!_musicState) {
        music = Variables::MUSIC_OFF_BUTTON;
    }

    if (!_effectsState) {
        effects = Variables::EFFECTS_OFF_BUTTON;
    }

    _musicButton = cocos2d::ui::Button::create();
    _musicButton->loadTextureNormal(music, cocos2d::ui::Widget::TextureResType::PLIST);

    _musicButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();
                _musicState = !_musicState;
                def->setBoolForKey("MUSIC", _musicState);
                this->_reloadButtons();
            }
                break;
            default:
                break;
        }
    });

    _musicButton->setPosition(Vec2(-_musicButton->getBoundingBox().size.width,
                                   _title->getPosition().y - _musicButton->getContentSize().height));

    this->addChild(_musicButton);

    _effectsButton = cocos2d::ui::Button::create();
    _effectsButton->loadTextureNormal(effects, cocos2d::ui::Widget::TextureResType::PLIST);
    _effectsButton->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();
                _effectsState = !_effectsState;
                def->setBoolForKey("EFFECTS", _effectsState);
                this->_reloadButtons();
            }
                break;
            default:
                break;
        }
    });
    _effectsButton->setPosition(Vec2(_effectsButton->getBoundingBox().size.width, _musicButton->getPosition().y));
    this->addChild(_effectsButton);
}

void PausePopUp::noAction() {
    PopUp::noAction();
}

void PausePopUp::yesAction() {
    PopUp::yesAction();
}


void InvitePopUp::noAction() {
    if (auto lobby = dynamic_cast<Lobby *>(this->getParent())) {
        lobby->denyInvite();
    }
    this->removeFromParent();
}

void InvitePopUp::yesAction() {
    if (auto lobby = dynamic_cast<Lobby *>(this->getParent())) {
        lobby->acceptInvite();
    }
    this->removeFromParent();
}

void InvitePopUp::okAction() {

}

InvitePopUp *InvitePopUp::create(std::string title, cocos2d::Node *message, bool isTwoButtons) {
    InvitePopUp *ret = new(std::nothrow) InvitePopUp();
    if (ret && ret->init(title, message, isTwoButtons)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

InvitePopUp *InvitePopUp::create(std::string title, cocos2d::Node *message) {
    InvitePopUp *ret = new(std::nothrow) InvitePopUp();
    if (ret && ret->init(title, message)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool InvitePopUp::init(std::string title, cocos2d::Node *message) {
    if(!PopUp::init(title, message)){
        return false;
    }

    message->setPosition(0, _title->getPosition().y - _title->getBoundingBox().size.height);

    return true;
}

bool InvitePopUp::init(std::string title, cocos2d::Node *message, bool isTwoButtons) {
    if(!PopUp::init(title, message, isTwoButtons)){
        return false;
    }

    message->setPosition(0, _title->getPosition().y - _title->getBoundingBox().size.height);

    return true;
}

GameTypePopUp *GameTypePopUp::create() {
    GameTypePopUp *ret = new(std::nothrow) GameTypePopUp();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GameTypePopUp::init() {
    if (!PopUp::init(LocalizedStrings::getInstance()->getString("CHOOSE GAME"))) {
        return false;
    }
    auto duel = ui::Button::create();
    duel->loadTextures(Variables::RED_BUTTON, Variables::RED_PRESSED_BUTTON, Variables::RED_BUTTON,
                           cocos2d::ui::Widget::TextureResType::PLIST);
    duel->setPosition(Vec2(0.f, 0.f));

    duel->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                if (auto lobby = dynamic_cast<Lobby *>(this->getParent())) {
                    if(_name == "" || _id){
                        lobby->startSearch(4);
                    } else {
                        lobby->inviteFriend(_name, _id, 4);
                    }
                }
                this->removeFromParent();
            }
                break;
            default:
                break;
        }
    });

    auto duelTitle = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("DUEL"), Variables::FONT_NAME,
                                                       Variables::FONT_SIZE());
    duelTitle->setPosition(duel->getContentSize().width / 2,
                               duel->getContentSize().height / 2);
    duel->addChild(duelTitle, 4);

    this->addChild(duel);

    auto apple = ui::Button::create();
    apple->loadTextures(Variables::BLUE_BUTTON, Variables::BLUE_PRESSED_BUTTON, Variables::BLUE_BUTTON,
                       cocos2d::ui::Widget::TextureResType::PLIST);
    apple->setPosition(Vec2(0.f, duel->getPosition().y - 3 * duel->getBoundingBox().size.height / 2));

    apple->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                if (auto lobby = dynamic_cast<Lobby *>(this->getParent())) {
                    if(_name == "" || _id){
                        lobby->startSearch(5);
                    } else {
                        lobby->inviteFriend(_name, _id, 5);
                    }
                }
                this->removeFromParent();
            }
                break;
            default:
                break;
        }
    });

    auto appleTitle = cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("APPLE"), Variables::FONT_NAME,
                                                   Variables::FONT_SIZE());
    appleTitle->setPosition(apple->getContentSize().width / 2,
                           apple->getContentSize().height / 2);
    apple->addChild(appleTitle, 4);

    this->addChild(apple);

    return true;

}

GameTypePopUp *GameTypePopUp::create(std::string name, int id) {
    GameTypePopUp *ret = new(std::nothrow) GameTypePopUp();
    if (ret && ret->init(name, id)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GameTypePopUp::init(std::string name, int id) {
    _name = name;
    _id = id;

    return GameTypePopUp::init();
}

InputNamePopUp *InputNamePopUp::create() {
    InputNamePopUp *ret = new(std::nothrow) InputNamePopUp();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool InputNamePopUp::init() {
    if (!PopUp::init(LocalizedStrings::getInstance()->getString("INPUT NAME"))) {
        return false;
    }

    auto editBoxSize = Size(3 * POPUP_SIZE.width / 4, 1.1f * Variables::H_FONT_SIZE());

    _editName = ui::EditBox::create(editBoxSize, Variables::BAR, ui::Widget::TextureResType::PLIST);
    _editName->setPosition(Vec2(0, 0));
    _editName->setFontName(Variables::FONT_NAME.c_str());
    _editName->setFontColor(Color3B::BLACK);
    _editName->setFontSize((int)Variables::H_FONT_SIZE());
    _editName->setMaxLength(12);
    _editName->setPlaceHolder(LocalizedStrings::getInstance()->getString("NAME"));
    _editName->setPlaceholderFontColor(Color3B::BLACK);
    _editName->setPlaceholderFontSize((int)Variables::H_FONT_SIZE());
    _editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editName->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    this->addChild(_editName, 3);


    auto ok = ui::Button::create();
    ok->loadTextures(Variables::YES_BUTTON_PATH, Variables::YES_PRESSED_BUTTON_PATH, Variables::YES_BUTTON_PATH,
                     cocos2d::ui::Widget::TextureResType::PLIST);
    ok->addTouchEventListener(CC_CALLBACK_0(InputNamePopUp::okAction, this));

    _buttons = Node::create();
    _buttons->addChild(ok);
    _buttons->setPosition(0, -POPUP_SIZE.height / 2 + ok->getBoundingBox().size.height / 2 + 25.f);

    _errorMessage = cocos2d::Label::createWithTTF("", Variables::FONT_NAME, Variables::FONT_SIZE());
    _errorMessage->setPosition(cocos2d::Vec2(
            0,
            _editName->getPosition().y - editBoxSize.height
    ));
    _errorMessage->setTextColor(Color4B::RED);
    this->addChild(_errorMessage, 3);

    this->addChild(_buttons, 2);
    return true;
}

void InputNamePopUp::okAction() {
    auto name  = string(_editName->getText());
    if(name.length() > 3){
        if (auto lobby = dynamic_cast<Lobby *>(this->getParent())) {
            lobby->addFriend(name);
        }
        this->removeFromParent();
    } else {
        _errorMessage ->setString(LocalizedStrings::getInstance()->getString("INPUT NAME"));
    }
}


WaitingPopUp *WaitingPopUp::create() {
    WaitingPopUp *ret = new(std::nothrow) WaitingPopUp();
    if (ret && ret->init(15)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

WaitingPopUp *WaitingPopUp::create(int time) {
    WaitingPopUp *ret = new(std::nothrow) WaitingPopUp();
    if (ret && ret->init(time)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool WaitingPopUp::init(int time) {
    _waitingTime = time;
    _message = Label::createWithTTF(StringUtils::toString(_waitingTime) + " s", Variables::FONT_NAME, Variables::H_FONT_SIZE());
    _message->setColor(Color3B::BLACK);

    if (!PopUp::init(LocalizedStrings::getInstance()->getString("WAITING"), _message)) {
        return false;
    }

    _ok = ui::Button::create();
    _ok->loadTextures(Variables::YES_BUTTON_PATH, Variables::YES_PRESSED_BUTTON_PATH, Variables::YES_DISABLED_PATH,
                     cocos2d::ui::Widget::TextureResType::PLIST);
    _ok->addTouchEventListener(CC_CALLBACK_0(WaitingPopUp::okAction, this));

    _ok->setEnabled(false);

    _buttons = Node::create();
    _buttons->addChild(_ok);
    _buttons->setPosition(0, -POPUP_SIZE.height / 2 + _ok->getBoundingBox().size.height / 2 + 25.f);

    this->addChild(_buttons, 2);
    this->schedule(SEL_SCHEDULE(&WaitingPopUp::update), 1.f);

    return true;
}

void WaitingPopUp::update(float dt) {
    Node::update(dt);
    if(_waitingTime - _counter == 0){
        _ok->setEnabled(true);
        _message->setString(LocalizedStrings::getInstance()->getString("LEAVE THE ROOM?"));
        this->unscheduleAllCallbacks();
    } else {
        _counter ++;
        _message->setString(StringUtils::toString(_waitingTime - _counter) + " s");
    }
}
