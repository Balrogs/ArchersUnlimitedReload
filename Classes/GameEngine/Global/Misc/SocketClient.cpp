#include <base/ccUTF8.h>
#include <Scenes/MenuLayers/MainMenu.h>
#include <Scenes/PlayLayers/Duel/DuelSceneMultiplayer.h>
#include <Scenes/MenuLayers/MultiplayerMenu.h>
#include <Scenes/MenuLayers/Lobby.h>
#include <Scenes/MenuLayers/Loading.h>
#include <Scenes/MenuLayers/RegisterMenu.h>
#include "JSONParser.h"

static SocketClient *instance = nullptr;


SocketClient *SocketClient::getInstance() {
    if (instance == nullptr) {
        instance = new SocketClient();
        if (!instance->connected()) {
            instance->_isConnected = instance->_conn(instance->_address, instance->_port);
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
    // _address = "127.0.0.1";
    _address = "188.120.237.20";
    _isConnected = false;
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

    char buffer[1024];
    string reply;

    //Receive a reply from the server
    if (recv(_sock, buffer, sizeof(buffer), 0) < 0) {
        puts("recv failed");
    }

    reply = buffer;
    reply = reply.substr(0, reply.find_last_of('}') + 1);
    string tmp;
    auto i = reply.find_first_of("}");
    for (;reply.length() > 0; i = reply.find_first_of("}")) {
        tmp.append(reply.substr(0, i + 1));
        reply = reply.substr(i + 1);
        if (reply.length() <= 0 || reply.at(0) == '{') {
            _buffer.push_back(tmp);
            tmp.clear();
        }
    }

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
    sprintf(x, "{\"name\":\"%s\",\"country\":%d,\"password\":\"%s\",\"code\":2}",
            name.c_str(),
            country,
            password.c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();

}

void SocketClient::enterRoom() {
    char x[256];
    sprintf(x,
            "{\"player_id\":%d,\"room_id\":%d,\"token\":{\"id\":%d,\"token\":\"%s\"},\"code\":4}",
            _player->getId(),
            _player->getRoomId(),
            _player->getId(),
            _player->getToken().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

void SocketClient::enterLobby(int lobbyId) {
    char x[256];
    sprintf(x,
            "{\"player_id\":%d,\"lobby_id\":%d,\"token\":{\"id\":%d,\"token\":\"%s\"},\"code\":40}",
            _player->getId(),
            lobbyId,
            _player->getId(),
            _player->getToken().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

void SocketClient::denyInvite() {
    char x[256];
    sprintf(x,
            "{\"player_id\":%d,\"room_id\":%d,\"token\":{\"id\":%d,\"token\":\"%s\"},\"code\":41}",
            _player->getId(),
            _player->getRoomId(),
            _player->getId(),
            _player->getToken().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

//{"winner_id":0, "room_id":1,"v_type":0,"token":{"id":1,"token":"顥鵰泏茙퍨ꅑꯛ೜㑓Ꮖ軼쭏籱ƹ槞⃥뚭薿咂튑"} ,"code":7}
void SocketClient::gameOver(int winner_id, int v_type) {
    char x[256];
    sprintf(x,
            "{\"winner_id\":%d, \"room_id\":%d,\"v_type\":%d,\"token\":{\"id\":%d,\"token\":\"%s\"} ,\"code\":7}",
            winner_id,
            _player->getRoomId(),
            v_type,
            _player->getId(),
            _player->getToken().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

void SocketClient::action(float angle, float power, int x, int y) {
    char mes[256];
    sprintf(mes,
            "{\"player_id\":%d,\"room_id\":%d,\"angle\":%f, \"power\":%f,\"x\":%d, \"y\":%d, \"token\":{\"id\":%d,\"token\":\"%s\"}, \"code\":60}",
            _player->getId(),
            _player->getRoomId(),
            angle,
            power,
            x,
            y,
            _player->getId(),
            _player->getToken().c_str());
    string message = mes;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();

}

void SocketClient::move(int dir) {
    char x[256];
    sprintf(x,
            "{\"player_id\":%d,\"room_id\":%d,\"dir\":%d,\"token\":{\"id\":%d,\"token\":\"%s\"}, \"code\":61}",
            _player->getId(),
            _player->getRoomId(),
            dir,
            _player->getId(),
            _player->getToken().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();

}

void SocketClient::aim(float angle, float power) {
    char x[256];
    sprintf(x,
            "{\"player_id\":%d,\"room_id\":%d,\"angle\":%f, \"power\":%f, \"token\":{\"id\":%d,\"token\":\"%s\"}, \"code\":62}",
            _player->getId(),
            _player->getRoomId(),
            angle,
            power,
            _player->getId(),
            _player->getToken().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();

}

void SocketClient::changeArrow(string arrow) {
    char x[256];
    sprintf(x,
            "{\"player_id\":%d,\"room_id\":%d, \"arrow\":\"%s\", \"token\":{\"id\":%d,\"token\":\"%s\"}, \"code\":60}",
            _player->getId(),
            _player->getRoomId(),
            arrow.c_str(),
            _player->getId(),
            _player->getToken().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();

}

void SocketClient::invite(int playerId) {
    char x[256];
    sprintf(x,
            "{\"id\":%d,\"password\":\"%s\",\"playerView\":{\"color\":{\"red\":0,\"green\":0,\"blue\":0},\"helmet\":0,\"hood\":0, \"bow\":0},\"code\":1}",
            _player->getId(),
            _player->getPassword().c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

void SocketClient::addToFriends(string name) {
    char x[256];
    sprintf(x,
            "{\"id\":%d, \"friend_name\":%s, \"code\":11}",
            _player->getId(),
            name.c_str());
    string message = x;
    auto t = std::thread(CC_CALLBACK_0(SocketClient::sendMessage, this, message));
    t.detach();
}

void SocketClient::getPlayerInfo(int s_type, string playerName) {
    char x[256];
    sprintf(x,
            "{\"s_type\":%d, \"name\":\"%s\",\"code\":9}",
            s_type,
            playerName.c_str());
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
                if (auto multiP = dynamic_cast<MultiplayerMenu *>(MainScene::getInstance()->getMain())) {
                    multiP->onError("Login failed! Incorrect name or password.");
                }
                if (auto loading = dynamic_cast<Loading *>(MainScene::getInstance()->getMain())) {
                    auto multiP = MultiplayerMenu::create();
                    MainScene::getInstance()->replaceMain(multiP);
                    multiP->onError("Login failed! Incorrect name or password.");
                }
            });
        }
            break;
        case -201: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto multiP = dynamic_cast<RegisterMenu *>(MainScene::getInstance()->getMain())) {
                    multiP->onError("Name is already taken!");
                }
            });
        }
            break;
        case -301: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto gameScene = dynamic_cast<MultiplayerBattle *>(BattleParent::getInstance())) {
                    gameScene->startGame();
                }
            });
        }
            break;
        case -302: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto gameScene = dynamic_cast<MultiplayerBattle *>(BattleParent::getInstance()))
                    gameScene->pauseGame();
            });
        }
            break;
        case -303: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto gameScene = dynamic_cast<MultiplayerBattle *>(BattleParent::getInstance()))
                    gameScene->resumeGame();
            });
        }
        case -304: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto gameScene = dynamic_cast<MultiplayerBattle *>(BattleParent::getInstance()))
                    gameScene->abort();
                else if (auto lobby = dynamic_cast<Lobby *>(MainScene::getInstance()->getMain())) {
                    lobby->deleteInvite();
                }
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
                if (auto gameScene = dynamic_cast<MultiplayerBattle *>(BattleParent::getInstance()))
                    gameScene->setPlayer(1);
            });
        }
            break;
        case -502: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto gameScene = dynamic_cast<MultiplayerBattle *>(BattleParent::getInstance()))
                    gameScene->setPlayer(2);
            });
        }
            break;
        case -600: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto lobby = dynamic_cast<Lobby *>(MainScene::getInstance()->getMain())) {
                    lobby->receivePlayerInfo("ERROR");
                }
            });
        }
            break;
        case -700: {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto lobby = dynamic_cast<Lobby *>(MainScene::getInstance()->getMain())) {
                    lobby->joinLobby();
                }
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
    if (instance == nullptr) {
        instance = new SocketClient();
    }
    return instance->_player;
}

void SocketClient::_parseReply(string reply) {
    if(!JSONParser::isValid(reply)){
        return;
    }
//    CCLOG(reply.c_str());
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
                    MainScene::getInstance()->replaceMain(Lobby::create());
                });
            }
                return;
            case 60: {
                auto power = JSONParser::parseFloat(reply, "power");
                auto angle = JSONParser::parseFloat(reply, "angle");
                auto id = JSONParser::parseIntAnswer(reply, "id");
                auto x = JSONParser::parseIntAnswer(reply, "x");
                auto y = JSONParser::parseIntAnswer(reply, "y");
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                    if (auto gameScene = dynamic_cast<MultiplayerBattle *>(BattleParent::getInstance()))
                        gameScene->receiveAction(angle, power, x, y, id);
                });
            }
                return;
            case 61: {
                auto dir = JSONParser::parseInt(reply, "dir");
                auto id = JSONParser::parseIntAnswer(reply, "id");
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                    if (auto gameScene = dynamic_cast<MultiplayerBattle *>(BattleParent::getInstance()))
                        gameScene->receiveMove(dir, id);
                });
            }
                return;
            case 62: {
                auto power = JSONParser::parseFloat(reply, "power");
                auto angle = JSONParser::parseFloat(reply, "angle");
                auto id = JSONParser::parseIntAnswer(reply, "id");
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                    if (auto gameScene = dynamic_cast<MultiplayerBattle *>(BattleParent::getInstance()))
                        gameScene->receiveAim(angle, power, id);
                });
            }
                return;
            case 9: {
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                    if (auto lobby = dynamic_cast<Lobby *>(MainScene::getInstance()->getMain())) {
                        lobby->receivePlayerInfo(reply);
                    }
                });
            }
                return;
            default:
                break;
        }

        if (!JSONParser::parseAnswer(reply, "player_name").empty()) {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (auto lobby = dynamic_cast<Lobby *>(MainScene::getInstance()->getMain())) {
                    lobby->receiveInvite(reply);
                    _player->setRoomId(JSONParser::parseIntAnswer(reply, "room_id"));
                }

            });
        }

        if (JSONParser::parseIntAnswer(reply, "winner_id")) {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                    if (auto gameScene = dynamic_cast<MultiplayerBattle *>(BattleParent::getInstance()))
                        gameScene->abort();
                });
            });
        }
    }
}