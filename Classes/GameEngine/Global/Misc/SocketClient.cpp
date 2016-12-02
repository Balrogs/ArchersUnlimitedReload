
#include <base/ccUTF8.h>
#include <Scenes/MenuLayers/MainMenu.h>
#include <Scenes/PlayLayers/DuelSceneMultiplayer.h>
#include "JSONParser.h"

static SocketClient *instance = nullptr;


SocketClient *SocketClient::getInstance() {
    if (instance == nullptr) {
        instance = new SocketClient();
        if (!instance->connected()) {
            instance->_conn(instance->_address, instance->_port);
        }
    }

    return instance;
}

void SocketClient::destroyInstance() {
    if (instance == nullptr)
        return;
    if (instance->_isConnected)
        shutdown(instance->_sock, 2);
    instance = nullptr;
}


SocketClient::SocketClient() {
    _sock = -1;
    _port = 8888;
    _address = "127.0.0.1";
    _isConnected = _conn(_address, _port);
    _player = new DBPlayer();
}

bool SocketClient::_conn(string address, int port) {
    if (_sock == -1) {
        //Create socket
        _sock = socket(AF_INET, SOCK_STREAM, 0);
        if (_sock == -1) {
            perror("Could not create socket");
        }
    }

    if (inet_addr(address.c_str()) == -1) {
        struct hostent *he;
        struct in_addr **addr_list;

        if ((he = gethostbyname(address.c_str())) == NULL) {
            //gethostbyname failed
            herror("gethostbyname");
            return false;
        }
        addr_list = (struct in_addr **) he->h_addr_list;

        for (int i = 0; addr_list[i] != NULL; i++) {
            _server.sin_addr = *addr_list[i];
            break;
        }
    } else {
        _server.sin_addr.s_addr = inet_addr(address.c_str());
    }

    _server.sin_family = AF_INET;
    _server.sin_port = htons(port);

    if (connect(_sock, (struct sockaddr *) &_server, sizeof(_server)) < 0) {
        perror("connection failed. Error");
        return false;
    }
    auto t = std::thread(CC_CALLBACK_0(SocketClient::receive, this));
    t.detach();
    return true;
}

/**
    Send data to the connected host
*/
bool SocketClient::sendMessage(string data) {
    return send(_sock, data.c_str(), strlen(data.c_str()), 0) != 0;
}

/**
    Receive data from the connected host
*/
void SocketClient::receive() {

    char buffer[512];
    string reply;

    //Receive a reply from the server
    if (recv(_sock, buffer, sizeof(buffer), 0) < 0) {
        puts("recv failed");
    }

    reply = buffer;
    reply = reply.substr(0, reply.find_last_of('}') + 1);

    auto ind = reply.find_first_of("}");
    for (; ind > 0 && ind + 1 < reply.length(); ind = reply.find_first_of("}", ind + 1)) {
        if (reply.at(ind + 1) == '{') {
            _buffer.push_back(reply.substr(0, ind + 1));
            reply = reply.substr(ind + 1);
        }
    }
    _buffer.push_back(reply);

    for (long i = _buffer.size() - 1; i >= 0; i--) {
        auto message = _buffer[i];
        if (!message.empty())
            _parseReply(message);
        _buffer.pop_back();
    }
    memset(&buffer[0], 0, sizeof(buffer));

    auto t = std::thread(CC_CALLBACK_0(SocketClient::receive, this));
    t.detach();
}

void SocketClient::login() {
    char x[256];
    sprintf(x,
            "{\"name\":\"%s\",\"password\":\"%s\",\"playerView\":{\"color\":{\"red\":0,\"green\":0,\"blue\":0},\"helmet\":0,\"hood\":0, \"bow\":0},\"code\":1}",
            _player->getName().c_str(), _player->getPassword().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

void SocketClient::registerUser(string name, int country, string password) {
    _player = new DBPlayer(-1, password, name, country);
    char x[256];
    sprintf(x, "{\"name\":\"%s\",\"country\":%s,\"password\":\"%s\",\"code\":2}", name.c_str(),
            cocos2d::StringUtils::toString(country).c_str(), password.c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();

}

void SocketClient::enterRoom() {
    char x[256];
    sprintf(x,
            "{\"player_id\":%s,\"room_id\":%s,\"token\":{\"id\":%s,\"token\":\"%s\"},\"code\":4}",
            cocos2d::StringUtils::toString(_player->getId()).c_str(),
            cocos2d::StringUtils::toString(_player->getRoomId()).c_str(),
            cocos2d::StringUtils::toString(_player->getId()).c_str(),
            _player->getToken().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

void SocketClient::enterLobby() {
    char x[256];
    sprintf(x,
            "{\"player_id\":%s,\"room_id\":%s,\"token\":{\"id\":%s,\"token\":\"%s\"},\"code\":40}",
            cocos2d::StringUtils::toString(_player->getId()).c_str(),
            cocos2d::StringUtils::toString(_player->getRoomId()).c_str(),
            cocos2d::StringUtils::toString(_player->getId()).c_str(),
            _player->getToken().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

void SocketClient::denyInvite() {
    char x[256];
    sprintf(x,
            "{\"player_id\":%s,\"room_id\":%s,\"token\":{\"id\":%s,\"token\":\"%s\"},\"code\":41}",
            cocos2d::StringUtils::toString(_player->getId()).c_str(),
            cocos2d::StringUtils::toString(_player->getRoomId()).c_str(),
            cocos2d::StringUtils::toString(_player->getId()).c_str(),
            _player->getToken().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

//{"winner_id":0, "room_id":1,"v_type":0,"token":{"id":1,"token":"顥鵰泏茙퍨ꅑꯛ೜㑓Ꮖ軼쭏籱ƹ槞⃥뚭薿咂튑"} ,"code":7}
void SocketClient::gameOver(int winner_id, int v_type) {
    char x[256];
    sprintf(x,
            "{\"winner_id\":%s, \"room_id\":%s,\"v_type\":%s,\"token\":{\"id\":%s,\"token\":\"%s\"} ,\"code\":7}",
            cocos2d::StringUtils::toString(winner_id).c_str(),
            cocos2d::StringUtils::toString(_player->getRoomId()).c_str(),
            cocos2d::StringUtils::toString(v_type).c_str(),
            cocos2d::StringUtils::toString(_player->getId()).c_str(),
            _player->getToken().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

void SocketClient::action(float angle, float power, int type) {
    char x[256];
    sprintf(x,
            "{\"player_id\":%s,\"room_id\":%s,\"angle\":%s, \"power\":%s, \"arrow\":\"%s\", \"token\":{\"id\":%s,\"token\":\"%s\"}, \"code\":6}",
            cocos2d::StringUtils::toString(_player->getId()).c_str(),
            cocos2d::StringUtils::toString(_player->getRoomId()).c_str(),
            cocos2d::StringUtils::toString(angle).c_str(),
            cocos2d::StringUtils::toString(power).c_str(),
            cocos2d::StringUtils::toString(type).c_str(),
            cocos2d::StringUtils::toString(_player->getId()).c_str(),
            _player->getToken().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();

}

void SocketClient::invite(int playerId) {
    char x[256];
    sprintf(x,
            "{\"id\":%s,\"password\":\"%s\",\"playerView\":{\"color\":{\"red\":0,\"green\":0,\"blue\":0},\"helmet\":0,\"hood\":0, \"bow\":0},\"code\":1}",
            cocos2d::StringUtils::toString(_player->getId()).c_str(), _player->getPassword().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

void SocketClient::addToFriends(int friendId) {
    char x[256];
    sprintf(x,
            "{\"id\":%s, \"friend_id\":%s, \"code\":11}",
            cocos2d::StringUtils::toString(_player->getId()).c_str(),
            cocos2d::StringUtils::toString(friendId).c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

void SocketClient::getPlayerInfo(int s_type, string playerName) {
    char x[256];
    sprintf(x,
            "{\"s_type\":%s, \"name\":\"%s\",\"code\":9}",
            cocos2d::StringUtils::toString(s_type).c_str(), playerName.c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

/*
 * > 0  -id пользователя в случае успешной авторизации
•
-100 — ошибка авторизации, неверный логин\пароль
•
-20x — ошибка регистрации
◦
-201 — имя пользователя занято
•
-30x — игровые события
◦
-301 — игра начата
◦
-302 — пауза
◦
-303 — продолжение игры
•
-400 — ошибка связи с сервером
•
-500 — вход в комнату
•
-600 — статистика пользователя не доступна. Пользователь не найден.
 */

void SocketClient::_parseError(int error) {
    switch (error) {
        case -100: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                MultiplayerMainMenu::getInstance()->onError("Login failed! Incorrect name or password.");
            });
        }
            break;
        case -201: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                MultiplayerMainMenu::getInstance()->onError("Name is already taken!");
            });
        }
            break;
        case -301: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto gameScene = dynamic_cast<DuelSceneMultiplayer *>(BattleScene::instance)) {
                    gameScene->startGame();
                }
            });
        }
            break;
        case -302: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto gameScene = dynamic_cast<DuelSceneMultiplayer *>(BattleScene::instance))
                    gameScene->pauseGame();
            });
        }
            break;
        case -303: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto gameScene = dynamic_cast<DuelSceneMultiplayer *>(BattleScene::instance))
                    gameScene->resumeGame();
            });
        }
        case -304: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto gameScene = dynamic_cast<DuelSceneMultiplayer *>(BattleScene::instance))
                    gameScene->abort();
                else
                    LobbyLayer::getInstance()->deleteInvite();
            });
        }
            break;
        case -400: {
            //TODO show error message
            cocos2d::Director::getInstance()->popToRootScene();
        }
            break;
        case -501: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto gameScene = dynamic_cast<DuelSceneMultiplayer *>(BattleScene::instance))
                    gameScene->setPlayer(1);
            });
        }
            break;
        case -502: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto gameScene = dynamic_cast<DuelSceneMultiplayer *>(BattleScene::instance))
                    gameScene->setPlayer(2);
            });
        }
            break;
        case -600: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                LobbyLayer::getInstance()->receivePlayerInfo("ERROR");
            });
        }
            break;
        case -700: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                LobbyLayer::getInstance()->joinLobby();
            });
        }
            break;
        default :
            break;
    }
}

bool SocketClient::connected() {
    return _isConnected;
}

DBPlayer *SocketClient::getDBPlayer() {
    return _player;
}

void SocketClient::_parseReply(string reply) {
    if (JSONParser::isError(reply)) {
        _parseError(atoi(JSONParser::parseError(reply, "answer").c_str()));
    } else {
        int code = JSONParser::parseIntAnswer(reply, "code");
        switch (code) {
            case 1:
                return;
            case 2:
                return;
            case 3: {
                _player->setId(JSONParser::parseInt(reply, "id"));
                _player->setToken(JSONParser::parse(reply, "token"));
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                    cocos2d::Director::getInstance()->replaceScene(LobbyLayer::createScene());
                });
            }
                return;
            case 6: {
                auto power = JSONParser::parseFloat(reply, "power");
                auto angle = JSONParser::parseFloat(reply, "angle");
                auto id = JSONParser::parseIntAnswer(reply, "id");
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                    if (auto gameScene = dynamic_cast<DuelSceneMultiplayer *>(BattleScene::instance))
                        gameScene->receiveAction(angle, power, id);
                });
            }
                return;
            case 9: {
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                    LobbyLayer::getInstance()->receivePlayerInfo(reply);
                });
            }
                return;
            default:
                break;
        }
        if (!JSONParser::parseAnswer(reply, "player_name").empty()) {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                LobbyLayer::getInstance()->receiveInvite(reply);
                _player->setRoomId(JSONParser::parseIntAnswer(reply, "room_id"));
            });
        }
    }
}


DBPlayer::DBPlayer() {

    cocos2d::UserDefault *def = cocos2d::UserDefault::getInstance();

    _id = def->getIntegerForKey("ID", -1);
    _name = def->getStringForKey("NAME", "");
    _password = def->getStringForKey("PASSWORD", "");
    _token = def->getStringForKey("TOKEN", "");
    _roomId = def->getIntegerForKey("ROOMID", -1);
    _country = def->getIntegerForKey("COUNTRY", -1);


    def->flush();
}

DBPlayer::DBPlayer(int id, string password, string name, int country) {
    _id = id;
    _name = name;
    _password = password;
    _token = "";
    _roomId = -1;
    _country = country;

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

