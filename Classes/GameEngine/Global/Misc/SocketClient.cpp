
#include <base/ccUTF8.h>
#include "SocketClient.h"
#include "JSONParser.h"


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

    return true;
}

/**
    Send data to the connected host
*/
string SocketClient::_sendMessage(string data) {
    if (send(_sock, data.c_str(), strlen(data.c_str()), 0)) {
        return _receive(512);
    }
    return "";
}

/**
    Receive data from the connected host
*/
string SocketClient::_receive(int size = 512) {
    char buffer[size];
    string reply;

    //Receive a reply from the server
    if (recv(_sock, buffer, sizeof(buffer), 0) < 0) {
        puts("recv failed");
    }

    reply = buffer;
    reply = reply.substr(0, reply.find_last_of('}') + 1);
    auto ind = reply.find_first_of("}");
    auto len = reply.length();
    auto l = reply.at(ind + 1);
    for (; ind > 0 && ind + 2 < reply.length(); ind = reply.find_first_of("}", ind + 1)) {
        if (reply.at(ind + 1) == '{') {
            _buffer.push_back(reply.substr(0, ind + 1));
            reply = reply.substr(ind + 1);
        }
    }
    return reply;
}

string SocketClient::login() {
    char x[256];
    sprintf(x,
            "{\"id\":%s,\"password\":\"%s\",\"playerView\":{\"color\":{\"red\":0,\"green\":0,\"blue\":0},\"helmet\":0,\"hood\":0, \"bow\":0},\"code\":1}",
            cocos2d::StringUtils::toString(_player->getId()).c_str(), _player->getPassword().c_str());
    string message = x;
    auto answer = _sendMessage(message);
    if (!JSONParser::isError(answer)) {
        return "";
    } else {
        int error = atoi(JSONParser::parseError(answer, "answer").c_str());
        return parseError(error);
    }
}

string SocketClient::registerUser(string name, int country, string password) {
    char x[256];
    sprintf(x, "{\"name\":\"%s\",\"country\":%s,\"password\":\"%s\",\"code\":2}", name.c_str(),
            cocos2d::StringUtils::toString(country).c_str(), password.c_str());
    string message = x;
    auto answer = _sendMessage(message);
    if (!JSONParser::isError(answer)) {
        int id = JSONParser::parseInt(answer, "id");
        string token = JSONParser::parse(answer, "token");
        _player = new DBPlayer(id, password, name, country);
        _player->setToken(token);
        return "";
    } else {
        int error = atoi(JSONParser::parseError(answer, "answer").c_str());
        return parseError(error);
    }
}

string SocketClient::action(int roomId, float angle, float power, int type) {
    char x[256];
    sprintf(x,
            "{\"id\":%s,\"password\":\"%s\",\"playerView\":{\"color\":{\"red\":0,\"green\":0,\"blue\":0},\"helmet\":0,\"hood\":0, \"bow\":0},\"code\":1}",
            cocos2d::StringUtils::toString(_player->getId()).c_str(), _player->getPassword().c_str());
    string message = x;
    auto answer = _sendMessage(message);
    if (!JSONParser::isError(answer)) {
        return "";
    } else {
        int error = atoi(JSONParser::parseError(answer, "answer").c_str());
        return parseError(error);
    }
}

string SocketClient::invite(int roomId) {
    char x[256];
    sprintf(x,
            "{\"id\":%s,\"password\":\"%s\",\"playerView\":{\"color\":{\"red\":0,\"green\":0,\"blue\":0},\"helmet\":0,\"hood\":0, \"bow\":0},\"code\":1}",
            cocos2d::StringUtils::toString(_player->getId()).c_str(), _player->getPassword().c_str());
    string message = x;
    auto answer = _sendMessage(message);
    if (!JSONParser::isError(answer)) {
        return "";
    } else {
        int error = atoi(JSONParser::parseError(answer, "answer").c_str());
        return parseError(error);
    }
}

string SocketClient::addToFriends(int friendId) {
    char x[256];
    sprintf(x,
            "{\"id\":%s,\"password\":\"%s\",\"playerView\":{\"color\":{\"red\":0,\"green\":0,\"blue\":0},\"helmet\":0,\"hood\":0, \"bow\":0},\"code\":1}",
            cocos2d::StringUtils::toString(_player->getId()).c_str(), _player->getPassword().c_str());
    string message = x;
    auto answer = _sendMessage(message);
    if (!JSONParser::isError(answer)) {
        return "";
    } else {
        int error = atoi(JSONParser::parseError(answer, "answer").c_str());
        return parseError(error);
    }
}

string SocketClient::getPlayerInfo(int s_type, string playerName) {
    char x[256];
    sprintf(x,
            "{\"s_type\":%s, \"name\":\"%s\",\"code\":9}",
            cocos2d::StringUtils::toString(s_type).c_str(), playerName.c_str());
    string message = x;
    auto answer = _sendMessage(message);
    if (!JSONParser::isError(answer)) {
        return answer;
    } else {
        int error = atoi(JSONParser::parseError(answer, "answer").c_str());
        return parseError(error);
    }
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
string SocketClient::parseError(int error) {
    switch (error) {
        case -100:
            return "Login failed! Incorrect name or password.";
        case -201:
            return "Name is already taken!";
        case -301:
            return "GAME STARTED";
        case -302:
            return "PAUSE";
        case -303:
            return "CONTINUE";
        case -400:
            return "SERVER CONNECTION ERROR";
        case -500:
            return "Entering the room...";
        case -600:
            return "USER NOT FOUND";
        default :
            return "";
    }
}

bool SocketClient::connected() {
    return _isConnected;
}

string SocketClient::checkInvite() {
    auto answer = _receive(512);
    if (answer.empty()) {
        return "";
    }
    answer = answer.substr(0, answer.find_last_of('}') + 1);
    if (!JSONParser::isError(answer)) {
        return answer;
    } else {
        int error = atoi(JSONParser::parseError(answer, "answer").c_str());
        return parseError(error);
    }
}

int SocketClient::getId() {
    return _player->getId();
}

string SocketClient::getToken() {
    return _player->getToken();
}

string SocketClient::getName() {
    return _player->getName();
}

string SocketClient::getPassword() {
    return _player->getPassword();
}

int SocketClient::getRoomId() {
    return _player->getRoomId();
}

int SocketClient::getCountry() {
    return _player->getCountry();
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

string DBPlayer::getToken() {
    return _token;
}

string DBPlayer::getPassword() {
    return _password;
}

int DBPlayer::getRoomId() {
    return _roomId;
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

