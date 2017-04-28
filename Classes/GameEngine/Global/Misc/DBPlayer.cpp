#include "DBPlayer.h"
#include "cocos2d.h"

using namespace std;


DBPlayer::DBPlayer() {

    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    _id = def->getIntegerForKey("ID", 0);
    _name = def->getStringForKey("NAME", "Player");
    _password = def->getStringForKey("PASSWORD", "");
    _token = def->getStringForKey("TOKEN", "");
    _roomId = def->getIntegerForKey("ROOMID", -1);
    _country = def->getIntegerForKey("COUNTRY", 6);
    _view = PlayerView::readPlayerView();

    def->flush();
}

DBPlayer::DBPlayer(int id, string password, string name, int country) {
    _id = id;
    _name = name;
    _password = password;
    _token = "";
    _roomId = -1;
    _country = country;
    _view = PlayerView::readPlayerView();

    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    def->setIntegerForKey("ID", id);
    def->setStringForKey("NAME", name);
    def->setStringForKey("PASSWORD", password);
    def->setStringForKey("TOKEN", "");
    def->setIntegerForKey("ROOMID", -1);
    def->setIntegerForKey("COUNTRY", country);

    def->flush();
}

int DBPlayer::getId() {
    return _id;
}

void DBPlayer::setId(int id) {
    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    def->setIntegerForKey("ID", id);

    def->flush();
    _id = id;
}

string DBPlayer::getToken() {
    return _token;
}

string DBPlayer::getPassword() {
    return _password;
}

int DBPlayer::getRoomId() {
    return _roomId;
}

void DBPlayer::setName(string name) {
    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    def->setStringForKey("NAME", name);

    def->flush();
    _name = name;
}

void DBPlayer::setPassword(string password) {
    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    def->setStringForKey("PASSWORD", password);

    def->flush();
    _password = password;
}

void DBPlayer::setToken(string token) {
    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    def->setStringForKey("TOKEN", token);

    def->flush();
    _token = token;
}

void DBPlayer::setRoomId(int roomId) {
    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    def->setIntegerForKey("ROOMID", roomId);

    def->flush();

    _roomId = roomId;
}

string DBPlayer::getName() {
    return _name;
}

int DBPlayer::getCountry() {
    return _country;
}

bool DBPlayer::canLogin() {
    return !_password.empty();
}

PlayerView *DBPlayer::getView() {
    setView(PlayerView::readPlayerView());
    return _view;
}

void DBPlayer::setView(PlayerView *view) {
    _view = view;
}

void DBPlayer::setCountry(int country) {
    if(_country != country) {
        cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

        def->setIntegerForKey("COUNTRY", country);

        def->flush();

        _country = country;
    }
}

