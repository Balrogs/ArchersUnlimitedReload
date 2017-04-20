#ifndef ARCHERSUNLIMITED_SOCKETCLIENT_H
#define ARCHERSUNLIMITED_SOCKETCLIENT_H

#include<string.h>
#include<string>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include "cocos2d.h"
#include "DBPlayer.h"

using namespace std;

class SocketClient {

private:
    SocketClient();
    int _sock;
    string _address;
    int _port;
    struct sockaddr_in _server;
    bool _isConnected;
    DBPlayer* _player;

    std::vector<std::string> _buffer;

    bool _conn(string, int);
    bool sendMessage(string data);

    void _parseError(int error);
    void _parseReply(string reply);

    void receive();

public:

    static SocketClient* getInstance();
    static void destroyInstance();
    DBPlayer* getDBPlayer();

    bool connected();

    // {"id":1,"password":"wqwf","playerView":{"color":{"red":0,"green":0,"blue":0},"helmet":0,"hood":0, "bow":0},"code":1}
    void login();
    // {"name":"some username12","country":1,"password":"1234","code":2}
    void registerUser(string name, int country, string password);
    //{"player_name":"123","player_rank":1,"room_id":55,"token":{"id":-1,"token":"server"}}
    void enterRoom();

    void denyInvite();

    void enterLobby(int lobbyId);
    // {"player_id":0,"room_id":1,"angle":180.0, "power":100.0, "token":{"id":0,"token":"⥿貧ဵට觱鞻㊆ꢖ풶꿘儐➧責鄆읽领ྯք뤘櫨"}, "code":6}
    void action(float angle, float power, int x, int y);
    // {"player_id":0,"room_id":1,"dir":1, "token":{"id":0,"token":"⥿貧ဵට觱鞻㊆ꢖ풶꿘儐➧責鄆읽领ྯք뤘櫨"}, "code":6}
    void move(int dir);
    // {"player_id":0,"room_id":1,"angle":180.0, "power":100.0, "token":{"id":0,"token":"⥿貧ဵට觱鞻㊆ꢖ풶꿘儐➧責鄆읽领ྯք뤘櫨"}, "code":6}
    void aim(float angle, float power);
    // {"player_id":0,"room_id":1,"arrow":"name", "token":{"id":0,"token":"⥿貧ဵට觱鞻㊆ꢖ풶꿘儐➧責鄆읽领ྯք뤘櫨"}, "code":6}
    void changeArrow(string arrow);
    // {"player_id":1, "room_id":1,"token":{"id":1,"token":"顥鵰泏茙퍨ꅑꯛ೜㑓Ꮖ軼쭏籱ƹ槞⃥뚭薿咂튑"},"code":4}
    void invite(int playerId);
    // {"id":0, "friend_id":1, "code":11}
    void addToFriends(string name);
    //{"s_type":3, "name":"some other username","code":9}
    void getPlayerInfo(int s_type, string playerName);
    //{"winner_id":0, "room_id":1,"v_type":0,"token":{"id":1,"token":"顥鵰泏茙퍨ꅑꯛ೜㑓Ꮖ軼쭏籱ƹ槞⃥뚭薿咂튑"} ,"code":7}
    void gameOver(int winner_id, int v_type);

    void leaveLobby();
    void getEventInfo();
};


#endif //ARCHERSUNLIMITED_SOCKETCLIENT_H
